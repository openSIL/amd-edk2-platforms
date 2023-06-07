/** @file
  Implements AMD Platform SoC Library.
  Provides interface to Get/Set platform specific data.

  Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.

**/
#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/Acpi65.h>
#include <Library/AmdPlatformSocLib.h>

/**
  Get the platform specific IOAPIC information.

  NOTE: Caller will need to free structure once finished.

  @param  IoApicInfo  The IOAPIC information
  @param  IoApicCount Number of IOAPIC present

  @retval EFI_UNSUPPORTED         Platform do not support this function.

**/
EFI_STATUS
EFIAPI
GetIoApicInfo (
  IN OUT EFI_ACPI_6_5_IO_APIC_STRUCTURE  **IoApicInfo,
  IN OUT UINT8                           *IoApicCount
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get the platform PCIe configuration information.

  NOTE: Caller will need to free structure once finished.

  @param  RootBridge              The root bridge information
  @param  RootBridgeCount         Number of root bridges present

  @retval EFI_UNSUPPORTED         Platform do not support this function.

**/
EFI_STATUS
EFIAPI
GetPcieInfo (
  IN OUT AMD_PCI_ROOT_BRIDGE_OBJECT_INSTANCE  **RootBridge,
  IN OUT UINTN                                *RootBridgeCount
  )
{
  return EFI_UNSUPPORTED;
}
