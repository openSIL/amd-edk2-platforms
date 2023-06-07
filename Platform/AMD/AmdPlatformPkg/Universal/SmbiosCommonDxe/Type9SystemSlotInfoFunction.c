/** @file
  AMD SMBIOS Type 9 Record

  Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.

**/
#include <Library/PrintLib.h>
#include <Protocol/AmdCpmTableProtocol/AmdCpmTableProtocol.h>
#include <GnbDxio.h>
#include <Library/NbioCommonLibDxe.h>
#include <Library/PcieConfigLib.h>
#include <AmdPcieComplex.h>
#include "SmbiosCommon.h"

/**
  This function returns SBDF information for a given slot number.

  @param[in]  SlotNumInfo                Slot number to be provided.
  @param[out] SegInfo                    Segment number.
  @param[out] BusInfo                    Bus number.
  @param[out] DevFunInfo                 Bits 0-2 corresponds to function number & bits 3-7 corresponds
                                         to device number.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_INVALID_PARAMETER      One or many parameters are invalid.
  @retval EFI_NOT_FOUND              SBDF information is not found for the given slot number.

**/
EFI_STATUS
EFIAPI
SlotBdfInfo (
  IN  UINT16  *SlotNumInfo,
  OUT UINT16  *SegInfo,
  OUT UINT8   *BusInfo,
  OUT UINT8   *DevFunInfo
  )
{
  EFI_STATUS            Status;
  PCIE_PLATFORM_CONFIG  *Pcie;
  PCIE_COMPLEX_CONFIG   *ComplexList;
  PCIE_SILICON_CONFIG   *SiliconList;
  PCIE_WRAPPER_CONFIG   *WrapperList;
  PCIE_ENGINE_CONFIG    *EngineList;

  if ((SlotNumInfo == NULL) || (SegInfo == NULL) || (BusInfo == NULL) || (DevFunInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Pcie   = NULL;
  Status = PcieGetPcieDxe (&Pcie);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ComplexList = (PCIE_COMPLEX_CONFIG *)PcieConfigGetChild (DESCRIPTOR_COMPLEX, &Pcie->Header);

  while (ComplexList != NULL) {
    SiliconList = PcieConfigGetChildSilicon (ComplexList);
    while (SiliconList != NULL) {
      WrapperList = PcieConfigGetChildWrapper (SiliconList);
      while (WrapperList != NULL) {
        EngineList = PcieConfigGetChildEngine (WrapperList);
        while (EngineList != NULL) {
          if (EngineList->Type.Port.PortData.SlotNum == *SlotNumInfo) {
            *SegInfo    = EngineList->Type.Port.Address.Address.Segment & 0xFFFF;
            *BusInfo    = EngineList->Type.Port.Address.Address.Bus & 0xFF;
            *DevFunInfo = (((EngineList->Type.Port.Address.Address.Device) & 0x1F) << 3) |
                          ((EngineList->Type.Port.Address.Address.Function) & 0x7);
            return EFI_SUCCESS;
          }

          EngineList = PcieLibGetNextDescriptor (EngineList);
        }

        WrapperList = PcieLibGetNextDescriptor (WrapperList);
      }

      SiliconList = PcieLibGetNextDescriptor (SiliconList);
    }

    if ((ComplexList->Header.DescriptorFlags & DESCRIPTOR_TERMINATE_TOPOLOGY) == 0) {
      ComplexList++;
    } else {
      ComplexList = NULL;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  This function allocates and populate system slot smbios record (Type 9).

  @param  DxioPortPtr              Pointer to DXIO port descriptor.
  @param  SmbiosRecordPtr          Pointer to smbios type 9 record.

  @retval EFI_SUCCESS              All parameters were valid.
  @retval EFI_INVALID_PARAMETER    One or many parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES     Resource not available.

**/
EFI_STATUS
EFIAPI
CreateSmbiosSystemSlotRecord (
  IN DXIO_PORT_DESCRIPTOR    *DxioPortPtr,
  IN OUT SMBIOS_TABLE_TYPE9  **SmbiosRecordPtr
  )
{
  EFI_STATUS          Status;
  CHAR8               SlotDesignationStr[SMBIOS_STRING_MAX_LENGTH];
  UINTN               SlotDesStrLen;
  UINTN               TotalSize;
  UINTN               StringOffset;
  SMBIOS_TABLE_TYPE9  *SmbiosRecord;
  MISC_SLOT_CHARACTERISTICS1  *SlotChar1Ptr;
  MISC_SLOT_CHARACTERISTICS2  *SlotChar2Ptr;
  SMBIOS_TABLE_TYPE9_EXTENDED SmbiosRecordExtended;
  UINT16              SlotNumInfo;
  UINT16              SegInfo;
  UINT8               BusInfo;
  UINT8               DevFunInfo;

  Status     = EFI_SUCCESS;
  SegInfo    = 0xFFFF;
  BusInfo    = 0xFF;
  DevFunInfo = 0xFF;

  if ((DxioPortPtr == NULL) || (SmbiosRecordPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  SlotChar1Ptr = PcdGetPtr (PcdAmdSmbiosType9SlotCharacteristics1);
  SlotChar2Ptr = PcdGetPtr (PcdAmdSmbiosType9SlotCharacteristics2);

  SlotDesStrLen = AsciiSPrint (
                    SlotDesignationStr,
                    SMBIOS_STRING_MAX_LENGTH,
                    "PCIE-%d",
                    DxioPortPtr->Port.SlotNum
                    );

  // Two zeros following the last string.
  TotalSize    = sizeof (SMBIOS_TABLE_TYPE9) + sizeof(SMBIOS_TABLE_TYPE9_EXTENDED) + SlotDesStrLen + 2 ;
  SmbiosRecord = NULL;
  SmbiosRecord = AllocateZeroPool (TotalSize);
  if (SmbiosRecord == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
  } else {
    SmbiosRecord->Hdr.Type          = SMBIOS_TYPE_SYSTEM_SLOTS;
    SmbiosRecord->Hdr.Length        = sizeof (SMBIOS_TABLE_TYPE9) + sizeof(SMBIOS_TABLE_TYPE9_EXTENDED);
    SmbiosRecord->Hdr.Handle        = 0;
    SmbiosRecord->SlotDesignation   = 1;
    SmbiosRecordExtended.SlotHeight = SlotHeightUnknown;
    SmbiosRecordExtended.SlotPitch  = 0;
    // Currently only map PCIE slots in system slot table.
    if (DxioPortPtr->EngineData.EngineType == DxioPcieEngine) {
      switch (DxioPortPtr->Port.LinkSpeedCapability) {
        case DxioGenMaxSupported:
          SmbiosRecord->SlotType = SlotTypePCIExpressGen5;
          break;
        case DxioGen1:
          SmbiosRecord->SlotType = SlotTypePciExpress;
          break;
        case DxioGen2:
          SmbiosRecord->SlotType = SlotTypePciExpressGen2;
          break;
        case DxioGen3:
          SmbiosRecord->SlotType = SlotTypePciExpressGen3;
          break;
        case DxioGen4:
          SmbiosRecord->SlotType = SlotTypePciExpressGen4;
          break;
        case DxioGen5:
          SmbiosRecord->SlotType = SlotTypePCIExpressGen5;
          break;
        default:
          SmbiosRecord->SlotType = SlotTypePCIExpressGen5;
          break;
      }
    } else {
      SmbiosRecord->SlotType = SlotTypeOther;
    }

    switch (DxioPortPtr->EngineData.DxioEndLane -
            DxioPortPtr->EngineData.DxioStartLane)
    {
      case 15:
        SmbiosRecord->SlotDataBusWidth = SlotDataBusWidth16X;
        SmbiosRecord->DataBusWidth     = 16;
        break;
      case 7:
        SmbiosRecord->SlotDataBusWidth = SlotDataBusWidth8X;
        SmbiosRecord->DataBusWidth     = 8;
        break;
      case 3:
        SmbiosRecord->SlotDataBusWidth = SlotDataBusWidth4X;
        SmbiosRecord->DataBusWidth     = 4;
        break;
      case 1:
        SmbiosRecord->SlotDataBusWidth = SlotDataBusWidth2X;
        SmbiosRecord->DataBusWidth     = 2;
        break;
      default:
        SmbiosRecord->SlotDataBusWidth = SlotDataBusWidth1X;
        SmbiosRecord->DataBusWidth     = 1;
        break;
    }
    SmbiosRecordExtended.SlotPhysicalWidth = SmbiosRecord->SlotDataBusWidth;

    if (DxioPortPtr->Port.EndpointStatus == (DXIO_ENDPOINT_STATUS)EndpointDetect) {
      SmbiosRecord->CurrentUsage = SlotUsageInUse;
    } else if (DxioPortPtr->Port.EndpointStatus == (DXIO_ENDPOINT_STATUS)EndpointNotPresent) {
      SmbiosRecord->CurrentUsage = SlotUsageAvailable;
    } else {
      SmbiosRecord->CurrentUsage = SlotUsageUnknown;
    }

    SlotNumInfo = DxioPortPtr->Port.SlotNum;
    Status      = SlotBdfInfo (
                    &SlotNumInfo,
                    &SegInfo,
                    &BusInfo,
                    &DevFunInfo
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Could not get SBDF information %r\n", Status));
    }
    SmbiosRecord->SlotLength           = SlotLengthUnknown;
    SmbiosRecord->SlotID               = DxioPortPtr->Port.SlotNum;
    CopyMem (&SmbiosRecord->SlotCharacteristics1, SlotChar1Ptr, sizeof(MISC_SLOT_CHARACTERISTICS1));
    CopyMem (&SmbiosRecord->SlotCharacteristics2, SlotChar2Ptr, sizeof(MISC_SLOT_CHARACTERISTICS2));
    SmbiosRecord->SegmentGroupNum      = SegInfo;
    SmbiosRecord->BusNum               = BusInfo;
    SmbiosRecord->DevFuncNum           = DevFunInfo;
    SmbiosRecord->PeerGroupingCount    = 0;
    StringOffset                       = SmbiosRecord->Hdr.Length;

    CopyMem ((UINT8 *)SmbiosRecord->PeerGroups + SmbiosRecord->PeerGroupingCount * sizeof(SmbiosRecord->PeerGroups), (UINT8 *)&SmbiosRecordExtended, sizeof(SMBIOS_TABLE_TYPE9_EXTENDED));
    CopyMem ((UINT8 *)SmbiosRecord + StringOffset, SlotDesignationStr, SlotDesStrLen);

    *SmbiosRecordPtr = SmbiosRecord;
  }

  return Status;
}

/**
  This function checks for system slot info and adds smbios record (Type 9).

  @param[in]  Smbios                     The EFI_SMBIOS_PROTOCOL instance.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_OUT_OF_RESOURCES       Resource not available.

**/
EFI_STATUS
EFIAPI
SystemSlotInfoFunction (
  IN  EFI_SMBIOS_PROTOCOL  *Smbios
  )
{
  EFI_STATUS                   Status;
  EFI_SMBIOS_HANDLE            SmbiosHandle;
  SMBIOS_TABLE_TYPE9           *SmbiosRecord;
  AMD_CPM_TABLE_PROTOCOL       *CpmTableProtocolPtr;
  AMD_CPM_DXIO_TOPOLOGY_TABLE  *DxioTopologyTablePtr2[2];
  UINTN                        DxioPortIdx;
  UINTN                        SocketIdx;

  if (Smbios == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gAmdCpmTableProtocolGuid,
                  NULL,
                  (VOID **)&CpmTableProtocolPtr
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate AmdCpmTableProtocol: %r\n", Status));
    return Status;
  }

  DxioTopologyTablePtr2[0] = NULL;
  DxioTopologyTablePtr2[0] = CpmTableProtocolPtr->CommonFunction.GetTablePtr2 (
                                                                   CpmTableProtocolPtr,
                                                                   CPM_SIGNATURE_DXIO_TOPOLOGY
                                                                   );

  DxioTopologyTablePtr2[1] = NULL;
  DxioTopologyTablePtr2[1] = CpmTableProtocolPtr->CommonFunction.GetTablePtr2 (
                                                                   CpmTableProtocolPtr,
                                                                   CPM_SIGNATURE_DXIO_TOPOLOGY_S1
                                                                   );

  // Add Smbios System Slot information for all sockets present.
  for (SocketIdx = 0; SocketIdx < FixedPcdGet32 (PcdAmdNumberOfPhysicalSocket); SocketIdx++ ) {
    if (DxioTopologyTablePtr2[SocketIdx] != NULL) {
      for (DxioPortIdx = 0; DxioPortIdx < AMD_DXIO_PORT_DESCRIPTOR_SIZE;
           DxioPortIdx++)
      {
        // Check if Slot is present
        if ((DxioTopologyTablePtr2[SocketIdx]->Port[DxioPortIdx].Port.SlotNum > 0) &&
            (DxioTopologyTablePtr2[SocketIdx]->Port[DxioPortIdx].Port.PortPresent == 1))
        {
          SmbiosRecord = NULL;
          Status       = CreateSmbiosSystemSlotRecord (
                           &DxioTopologyTablePtr2[SocketIdx]->Port[DxioPortIdx],
                           &SmbiosRecord
                           );

          if (EFI_ERROR (Status)) {
            DEBUG ((
              DEBUG_ERROR,
              "%a: Smbios system slot error: Status=%r\n",
              __FUNCTION__,
              Status
              ));
          } else {
            Status = AddCommonSmbiosRecord (
                       Smbios,
                       &SmbiosHandle,
                       (EFI_SMBIOS_TABLE_HEADER *)SmbiosRecord
                       );
          }

          if (SmbiosRecord != NULL) {
            FreePool (SmbiosRecord);
          }
        }

        // Terminate if last port found.
        if ((DxioTopologyTablePtr2[SocketIdx]->Port[DxioPortIdx].Flags & 0x80000000)) {
          break;
        }
      }
    }
  }

  return Status;
}
