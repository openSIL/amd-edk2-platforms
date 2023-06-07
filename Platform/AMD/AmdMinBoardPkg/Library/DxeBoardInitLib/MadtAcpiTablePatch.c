/** @file

Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved

**/

#include "DxeBoardInitLibInternal.h"
#include <Library/AmdPlatformSocLib.h>

#define   AMD_CPUID_EXTENDED_TOPOLOGY_V2                 0x26
#define   AMD_CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_CCX  0x03
#define   AMD_CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_CCD  0x04
#define   AMD_CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_DIE  0x05

UINT32  mCcdOrder[16] = { 0, 4, 8, 12, 2, 6, 10, 14, 3, 7, 11, 15, 1, 5, 9, 13 };

/**
  Get Package ID/Die ID/Ccd ID/Complex ID/Core ID/Thread ID of a processor.

  This algorithm is derived from GetProcessorLocation2ByApicId
  for AMD processor extended topology extension.

  The algorithm assumes the target system has symmetry across physical
  package boundaries with respect to the number of threads per core, number of
  cores per complex, number of complex per Ccd, number of Ccd for Die and
  number of die per package.

  @param[in]   InitialApicId Initial APIC ID of the target logical processor.
  @param[out]  Package       Returns the processor package ID.
  @param[out]  Die           Returns the processor die ID.
  @param[out]  CCD           Returns the number of core complex die ID.
  @param[out]  Complex       Returns the processor CCX ID.
  @param[out]  Core          Returns the processor core ID.
  @param[out]  Thread        Returns the processor thread ID.
**/
VOID
EFIAPI
AmdGetProcessorLocation2ByApicId (
  IN  UINT32  InitialApicId,
  OUT UINT32  *Package  OPTIONAL,
  OUT UINT32  *Die      OPTIONAL,
  OUT UINT32  *Ccd      OPTIONAL,
  OUT UINT32  *Complex  OPTIONAL,
  OUT UINT32  *Core     OPTIONAL,
  OUT UINT32  *Thread   OPTIONAL
  )
{
  CPUID_EXTENDED_TOPOLOGY_EAX  ExtendedTopologyEax;
  CPUID_EXTENDED_TOPOLOGY_ECX  ExtendedTopologyEcx;
  UINT32                       Index;
  UINTN                        LevelType;
  UINT32                       Bits[AMD_CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_DIE + 2];
  UINT32                       *Location[AMD_CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_DIE + 2];

  for (LevelType = 0; LevelType < ARRAY_SIZE (Bits); LevelType++) {
    Bits[LevelType] = 0;
  }

  for (
       Index = 0;
       Index < (AMD_CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_DIE + 2);
       Index++
       )
  {
    AsmCpuidEx (
      (CPUID_EXTENDED_FUNCTION | AMD_CPUID_EXTENDED_TOPOLOGY_V2),
      Index,
      &ExtendedTopologyEax.Uint32,
      NULL,
      &ExtendedTopologyEcx.Uint32,
      NULL
      );
    if (ExtendedTopologyEcx.Bits.LevelType == \
        CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_INVALID)
    {
      break;
    }

    Bits[ExtendedTopologyEcx.Bits.LevelType] = \
      ExtendedTopologyEax.Bits.ApicIdShift;
  }

  Location[AMD_CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_DIE + 1] = Package;
  Location[AMD_CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_DIE]     = Die;
  Location[AMD_CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_CCD]     = Ccd;
  Location[AMD_CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_CCX]     = Complex;
  Location[CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_CORE]           = Core;
  Location[CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_SMT]            = Thread;

  for (
       LevelType = CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_CORE;
       LevelType < ARRAY_SIZE (Bits);
       LevelType++
       )
  {
    //
    // If there are more levels between level-1 (low-level) and
    // level-2 (high-level), the unknown levels will be ignored
    // and treated as an extension of the last known
    // level (i.e., level-1 in this case).
    //
    if (Bits[LevelType] == 0) {
      Bits[LevelType] = Bits[LevelType - 1];
    }
  }

  // single bit for package/socket
  Bits[AMD_CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_DIE + 1] = \
    Bits[AMD_CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_DIE] + 1;

  for ( LevelType = CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_SMT
        ; LevelType <= AMD_CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_DIE + 1
        ; LevelType++
        )
  {
    if (Location[LevelType] != NULL) {
      //
      // Bits[i] holds the number of bits to shift right on x2APIC ID to get
      // a unique topology ID of the next level type.
      //
      *Location[LevelType] = \
        (InitialApicId & ((1 << Bits[LevelType]) - 1)) >> Bits[LevelType - 1];
    }
  }

  // Always return zero for unsupported level
  if (Die != NULL) {
    *Die = 0; // AMD doenst implement Die
  }
}

/**
  Callback compare function.
  Compares CCD number of provided arguments.

  @param[in] LocalX2ApicLeft   Pointer to Left Buffer.
  @param[in] LocalX2ApicRight  Pointer to Right Buffer.
  @return    0                 If both are same
             -1                If left value is less than righ value.
             1                 If left value is greater than righ value.

**/
INTN
EFIAPI
SortByCcd (
  CONST VOID  *LocalX2ApicLeft,
  CONST VOID  *LocalX2ApicRight
  )
{
  CONST EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE  *Left;
  CONST EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE  *Right;
  EFI_CPU_PHYSICAL_LOCATION2                           LeftLocation;
  EFI_CPU_PHYSICAL_LOCATION2                           RightLocation;
  UINT32                                               LeftCcdIndex;
  UINT32                                               RightCcdIndex;
  UINT32                                               Index;

  Left  = (EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE *)LocalX2ApicLeft;
  Right = (EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE *)LocalX2ApicRight;

  AmdGetProcessorLocation2ByApicId (
    Left->X2ApicId,
    &LeftLocation.Package,
    &LeftLocation.Tile,
    &LeftLocation.Die,
    &LeftLocation.Module,
    &LeftLocation.Core,
    &LeftLocation.Thread
    );

  AmdGetProcessorLocation2ByApicId (
    Right->X2ApicId,
    &RightLocation.Package,
    &RightLocation.Tile,
    &RightLocation.Die,
    &RightLocation.Module,
    &RightLocation.Core,
    &RightLocation.Thread
    );

  // Get the CCD Index number
  LeftCcdIndex = MAX_UINT32;
  for (Index = 0; Index < ARRAY_SIZE (mCcdOrder); Index++) {
    if (LeftLocation.Die == mCcdOrder[Index]) {
      LeftCcdIndex = Index;
      break;
    }
  }

  RightCcdIndex = MAX_UINT32;
  for (Index = 0; Index < ARRAY_SIZE (mCcdOrder); Index++) {
    if (RightLocation.Die == mCcdOrder[Index]) {
      RightCcdIndex = Index;
      break;
    }
  }

  // Now compare for quick sort
  if (LeftCcdIndex < RightCcdIndex) {
    return -1;
  }

  if (LeftCcdIndex > RightCcdIndex) {
    return 1;
  }

  return 0;
}

/**
  A Callback function to patch the ACPI MADT table.
  Updates MADT table with AMD specific values, which
  are different than MinPlatformPkg.

  @param[in, out] NewTable       Pointer to ACPI MADT table

  @return         EFI_SUCCESS    Always return EFI_SUCCESSe

**/
EFI_STATUS
EFIAPI
MadtAcpiTablePatch (
  IN OUT  EFI_ACPI_SDT_HEADER  *NewTable
  )
{
  UINT32                                               Index;
  EFI_ACPI_6_5_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER  *NewMadtTable;
  UINT8                                                *TablePtr;
  UINT64                                               Length;
  EFI_ACPI_6_5_IO_APIC_STRUCTURE                       *NbioIoApic;
  UINT8                                                IoApicCount;
  UINTN                                                LapicCount;
  EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE        *LocalX2ApicPtr;
  EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE        *SortedItem;
  EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE        *Src;
  EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE        *Dst;

  // Patch the Table
  NewMadtTable                  = (EFI_ACPI_6_5_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER *)NewTable;
  NewMadtTable->Header.Revision = 6;
  // Get the IoApic information
  NbioIoApic     = NULL;
  IoApicCount    = 0;
  LapicCount     = 0;
  LocalX2ApicPtr = NULL;
  GetIoApicInfo (&NbioIoApic, &IoApicCount);
  if ((NbioIoApic == NULL) || (IoApicCount == 0)) {
    DEBUG ((DEBUG_INFO, "%a:%d Cannot obtain NBIO IOAPIC information.\n", __FUNCTION__, __LINE__));
    return EFI_SUCCESS;
  }

  // Create MADT header
  TablePtr = (UINT8 *)NewMadtTable;
  TablePtr = TablePtr + sizeof (EFI_ACPI_6_5_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER);
  Length   = sizeof (EFI_ACPI_6_5_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER);

  // Get the IOAPIC structure
  Index = 0;    // now holds the IoApic Index
  do {
    if (((STRUCTURE_HEADER *)TablePtr)->Type == EFI_ACPI_6_5_IO_APIC) {
      // Patch the IoApic Strucure
      if (Index >= IoApicCount) {
        /// Mark the extra IOAPIC structure Type as reserved, so that OSPM can ignore it.
        /// As per ACPI specification 6.5 for MADT table
        /// Subtype 0x18-0x7F are reserved, OSPM skips structures of the reserved type.
        ((EFI_ACPI_6_5_IO_APIC_STRUCTURE *)TablePtr)->Type                      = 0x7F;
      } else {
        ((EFI_ACPI_6_5_IO_APIC_STRUCTURE *)TablePtr)->IoApicId                  = NbioIoApic[Index].IoApicId;
        ((EFI_ACPI_6_5_IO_APIC_STRUCTURE *)TablePtr)->IoApicAddress             = NbioIoApic[Index].IoApicAddress;
        ((EFI_ACPI_6_5_IO_APIC_STRUCTURE *)TablePtr)->GlobalSystemInterruptBase = NbioIoApic[Index].GlobalSystemInterruptBase;
      }
      Index++;
    }

    if (((STRUCTURE_HEADER *)TablePtr)->Type == EFI_ACPI_6_5_INTERRUPT_SOURCE_OVERRIDE) {
      // Patch Flags
      ((EFI_ACPI_6_5_INTERRUPT_SOURCE_OVERRIDE_STRUCTURE *)TablePtr)->Flags = 0xF;
    }

    if (((STRUCTURE_HEADER *)TablePtr)->Type == EFI_ACPI_6_5_LOCAL_X2APIC_NMI) {
      // Patch  Flags - Edge-tiggered, Active High
      ((EFI_ACPI_6_5_LOCAL_X2APIC_NMI_STRUCTURE *)TablePtr)->Flags = 0x0005;
    }

    if (((STRUCTURE_HEADER *)TablePtr)->Type == EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC) {
      if (LapicCount == 0) {
        // Get the first entry pointer
        LocalX2ApicPtr = (EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE *)TablePtr;
      }

      LapicCount += 1;
    }

    Length   += ((STRUCTURE_HEADER *)TablePtr)->Length;
    TablePtr += ((STRUCTURE_HEADER *)TablePtr)->Length;
  } while (Length < NewMadtTable->Header.Length);

  FreePool (NbioIoApic);

  if (LocalX2ApicPtr != NULL) {
    if (FixedPcdGet32 (PcdMaxCpuSocketCount) > 1) {
      /// Sort by CCD location
      PerformQuickSort (LocalX2ApicPtr, LapicCount/2, sizeof (EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE), SortByCcd);
      PerformQuickSort (LocalX2ApicPtr+(LapicCount/2), LapicCount/2, sizeof (EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE), SortByCcd);
    } else {
      /// Sort by CCD location
      PerformQuickSort (LocalX2ApicPtr, LapicCount, sizeof (EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE), SortByCcd);
    }

    /// Now allocate the Uid
    SortedItem = LocalX2ApicPtr;
    for (Index = 0; Index < LapicCount; Index++, SortedItem++) {
      SortedItem->AcpiProcessorUid = Index;
    }

    // Now seperate the second thread list
    SortedItem = LocalX2ApicPtr + 1;
    if ((SortedItem->X2ApicId & 0x1) == 0x1) {
      // It has multi-thread on
      SortedItem = NULL;
      SortedItem = AllocateZeroPool (sizeof (EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE) * LapicCount);
      if (SortedItem == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }

      Src = LocalX2ApicPtr;
      Dst = SortedItem;
      for (Index = 0; Index < LapicCount; Index++) {
        if ((Src->X2ApicId & 0x1) == 0) {
          CopyMem (Dst, Src, sizeof (EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE));
          Src++;
          Dst++;
        } else {
          Src++;
        }
      }

      Src = LocalX2ApicPtr;
      for (Index = 0; Index < LapicCount; Index++) {
        if ((Src->X2ApicId & 0x1) == 1) {
          CopyMem (Dst, Src, sizeof (EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE));
          Src++;
          Dst++;
        } else {
          Src++;
        }
      }

      CopyMem (LocalX2ApicPtr, SortedItem, sizeof (EFI_ACPI_6_5_PROCESSOR_LOCAL_X2APIC_STRUCTURE) * LapicCount);
      FreePool (SortedItem);
    }
  }

  return EFI_SUCCESS;
}
