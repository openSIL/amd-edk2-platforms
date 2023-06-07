/** @file

  FV block I/O protocol driver for SPI flash libary.

  Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.

**/

#ifndef ACPI_COMMON_H_
#define ACPI_COMMON_H_

#include <IndustryStandard/Acpi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/AcpiTable.h>
#include <Uefi.h>

#define AMD_DSDT_OEMID         SIGNATURE_64 ('A', 'm', 'd', 'T','a','b','l','e')
#define CREATOR_REVISION       2
#define MAX_LOCAL_STRING_SIZE  20
#define OEM_REVISION_NUMBER    0

extern  EFI_ACPI_TABLE_PROTOCOL  *mAcpiTableProtocol;
extern  EFI_ACPI_SDT_PROTOCOL    *mAcpiSdtProtocol;

/**
  Appends generated AML to an existing ACPI Table

  1. Locate the existing ACPI table
  2. Allocate pool for original table plus new data size
  3. Copy original table to new buffer
  4. Append new data to buffer
  5. Update Table header length (Checksum will be calculated on install)
  6. Uninstall original ACPI table
  7. Install appended table
  8. Free new table buffer since ACPI made a copy.

  @param[in]      Signature     - The Acpi table signature
  @param[in]      OemId         - The Acpi table OEM ID
  @param[in]      AmlData       - The AML data to append

  @retval         EFI_SUCCESS, various EFI FAILUREs.
**/
EFI_STATUS
EFIAPI
AppendExistingAcpiTable (
  IN      UINT32                       Signature,
  IN      UINT64                       OemId,
  IN      EFI_ACPI_DESCRIPTION_HEADER  *AmlData
  );

/**
  Install CPU devices scoped under \_SB into DSDT

  Determine all the CPU threads and create ACPI Device nodes for each thread.
  AGESA will scope to these CPU records when installing CPU power and
  performance capabilities.

  @param[in]      ImageHandle   - Standard UEFI entry point Image Handle
  @param[in]      SystemTable   - Standard UEFI entry point System Table

  @retval         EFI_SUCCESS, various EFI FAILUREs.
**/
EFI_STATUS
EFIAPI
InstallCpuAcpi (
  IN      EFI_HANDLE        ImageHandle,
  IN      EFI_SYSTEM_TABLE  *SystemTable
  );

/**
  Install PCI devices scoped under \_SB into DSDT

  Determine all the PCI Root Bridges and PCI root ports and install resources
  including needed _HID, _CID, _UID, _ADR, _CRS and _PRT Nodes.

  @param[in]      ImageHandle   - Standard UEFI entry point Image Handle
  @param[in]      SystemTable   - Standard UEFI entry point System Table

  @retval         EFI_SUCCESS, various EFI FAILUREs.
**/
EFI_STATUS
EFIAPI
InstallPciAcpi (
  IN      EFI_HANDLE        ImageHandle,
  IN      EFI_SYSTEM_TABLE  *SystemTable
  );

VOID
EFIAPI
InstallAcpiSpmiTable (
  VOID
  );

#endif // ACPI_COMMON_H__
