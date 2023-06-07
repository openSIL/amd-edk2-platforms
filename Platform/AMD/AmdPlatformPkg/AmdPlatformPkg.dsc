## @file
# AMD Platform common Package DSC file
# This is the package provides the AMD edk2 common platform drivers
# and libraries for AMD Server, Clinet and Gaming console platforms.
#
# Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.<BR>
#
##

[Defines]
  PLATFORM_NAME                  = AmdPlatformPkg
  PLATFORM_GUID                  = ACFD1C98-D451-45FE-B300-4049C5AD553B
  PLATFORM_VERSION               = 1.0
  DSC_SPECIFICATION              = 1.28
  OUTPUT_DIRECTORY               = Build/AmdPlatformPkg
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT

[Packages]
  AmdPlatformPkg/AmdPlatformPkg.dec

!include MdePkg/MdeLibs.dsc.inc

# Include AGESA module for edk2-platforms
!include AgesaModulePkg/AgesaEdk2PlatformPkg.inc.dsc

[LibraryClasses.Common]
  AmlLib|DynamicTablesPkg/Library/Common/AmlLib/AmlLib.inf
  AcpiHelperLib|DynamicTablesPkg/Library/Common/AcpiHelperLib/AcpiHelperLib.inf
  AmdPspMboxLibV2|AgesaModulePkg/Library/AmdPspMboxLibV2/AmdPspMboxLibV2.inf
  AmdPspBaseLibV2|AgesaModulePkg/Library/AmdPspBaseLibV2/AmdPspBaseLibV2.inf
  AmdPspMmioLib|AgesaModulePkg/Library/AmdPspMmioLib/AmdPspMmioLib.inf
  AmdPspRegBaseLib|AgesaModulePkg/Library/AmdPspRegBaseLib/AmdPspRegBaseLib.inf
  AmdSocBaseLib|AgesaModulePkg/Library/AmdSocBaseLib/AmdSocBaseLib.inf
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapLibNull/AmdHeapLibNull.inf
  AmdPspRegMuxLibV2|AgesaModulePkg/Library/AmdPspRegMuxLibV2Null/AmdPspRegMuxLibV2.inf
  AmdDirectoryBaseLib|AgesaModulePkg/Library/AmdDirectoryBaseLib/AmdDirectoryBaseLib.inf
  FchBaseLib|AgesaModulePkg/Library/FchBaseLib/FchBaseLib.inf
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom2Lib.inf

  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  AmlGenerationLib|AgesaModulePkg/Library/DxeAmlGenerationLib/AmlGenerationLib.inf
  SmnAccessLib|AgesaModulePkg/Library/SmnAccessLib/SmnAccessLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf

  #
  # Debug libraries
  #
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
!if $(TARGET) == RELEASE
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
!else
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif

  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf

  #
  # CryptLib
  #
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf

  #
  # Secureboot library
  #
  SecureBootVariableLib|SecurityPkg/Library/SecureBootVariableLib/SecureBootVariableLib.inf
  SecureBootVariableProvisionLib|SecurityPkg/Library/SecureBootVariableProvisionLib/SecureBootVariableProvisionLib.inf
  PlatformPKProtectionLib|SecurityPkg/Library/PlatformPKProtectionLibVarPolicy/PlatformPKProtectionLibVarPolicy.inf

  #
  # Platform - Replace the modules in this section in platform DSC file.
  #
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf

  # Simulator serial port 80
  SerialPortLib|AmdPlatformPkg/Library/SimulatorSerialPortLibPort80/SimulatorSerialPortLibPort80.inf

  # Boot logo
  BootLogoLib|MdeModulePkg/Library/BootLogoLib/BootLogoLib.inf

  NbioHandleLib|AgesaModulePkg/Library/NbioHandleLib/NbioHandleLib.inf
  PcieConfigLib|AgesaModulePkg/Library/PcieConfigLib/PcieConfigLib.inf

[LibraryClasses.common.DXE_CORE, LibraryClasses.common.DXE_SMM_DRIVER, LibraryClasses.common.SMM_CORE, LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.DXE_RUNTIME_DRIVER, LibraryClasses.common.UEFI_DRIVER, LibraryClasses.common.UEFI_APPLICATION]
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  SmmServicesTableLib|MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
  AmdPspRomArmorLib|AgesaModulePkg/Library/AmdPspRomArmorLibNull/AmdPspRomArmorLibNull.inf
  PlatformPspRomArmorWhitelistLib|AgesaPkg/Addendum/Psp/PspRomArmorWhitelistLib/PspRomArmorWhitelistLib.inf

[LibraryClasses.Common.DXE_DRIVER]
  AmdPspRomArmorLib|AgesaModulePkg/Library/AmdPspRomArmorLibNull/AmdPspRomArmorLibNull.inf
  PlatformSocLib|AmdPlatformPkg/Library/DxePlatformSocLib/DxePlatformSocLibNull.inf

[LibraryClasses.Common.SMM_CORE]
  SmmCorePlatformHookLib|AmdPlatformPkg/Library/SmmCorePlatformHookLib/SmmCorePlatformHookLib.inf
  SmmCoreAmdSpiHcHookHookLib|AmdPlatformPkg/Library/SmmCoreAmdSpiHcHookLib/SmmCoreAmdSpiHcHookLib.inf

[Components.common.DXE_DRIVER]
  AmdPlatformPkg/Universal/HiiConfigRouting/AmdConfigRouting.inf
  AmdPlatformPkg/Universal/SecureBoot/SecureBootDefaultKeysInit/SecureBootDefaultKeysInit.inf
  AmdPlatformPkg/Universal/LogoDxe/LogoDxe.inf                                               # Server platfrom Bitmap logo driver
  AmdPlatformPkg/Universal/LogoDxe/JpegLogoDxe.inf                                           # Server platform JPEG logo driver
  AmdPlatformPkg/Universal/SmbiosCommonDxe/SmbiosCommonDxe.inf                               # AMD SMBIOS common DXE driver

[Components]
  AmdPlatformPkg/Library/SmmCorePlatformHookLib/SmmCorePlatformHookLib.inf
  AmdPlatformPkg/Library/SmmCoreAmdSpiHcHookLib/SmmCoreAmdSpiHcHookLib.inf
  AmdPlatformPkg/Library/SimulatorSerialPortLibPort80/SimulatorSerialPortLibPort80.inf
  AmdPlatformPkg/Library/BaseAlwaysFalseDepexLib/BaseAlwaysFalseDepexLib.inf
  AgesaModulePkg/Nbio/Library/CommonDxe/NbioCommonDxeLib.inf
  AgesaModulePkg/Library/PcieConfigLib/PcieConfigLib.inf
  AmdPlatformPkg/Library/DxePlatformSocLib/DxePlatformSocLibNull.inf

[Components.X64]
  AmdPlatformPkg/Universal/Acpi/AcpiCommon/AcpiCommon.inf
  AmdPlatformPkg/Universal/Spi/AmdSpiFvb/AmdSpiFvbSmm.inf
  AmdPlatformPkg/Universal/Spi/AmdSpiFvb/AmdSpiFvbDxe.inf
  AmdPlatformPkg/Universal/Spi/AmdSpiHc/AmdSpiHcSmm.inf
  AmdPlatformPkg/Universal/Spi/AmdSpiHc/AmdSpiHcDxe.inf 
  AmdPlatformPkg/Universal/Spi/BoardSpiBus/BoardSpiBusDxe.inf
  AmdPlatformPkg/Universal/Spi/BoardSpiBus/BoardSpiBusSmm.inf
  AmdPlatformPkg/Universal/Spi/BoardSpiConfig/BoardSpiConfigDxe.inf
  AmdPlatformPkg/Universal/Spi/BoardSpiConfig/BoardSpiConfigSmm.inf

[BuildOptions]
  GCC:*_*_*_CC_FLAGS     = -D DISABLE_NEW_DEPRECATED_INTERFACES
  INTEL:*_*_*_CC_FLAGS   = /D DISABLE_NEW_DEPRECATED_INTERFACES
  MSFT:*_*_*_CC_FLAGS    = /D DISABLE_NEW_DEPRECATED_INTERFACES

  GCC:*_*_*_CC_FLAGS     = -D USE_EDKII_HEADER_FILE

  # Turn off DEBUG messages for Release Builds
  GCC:RELEASE_*_*_CC_FLAGS     = -D MDEPKG_NDEBUG
  INTEL:RELEASE_*_*_CC_FLAGS   = /D MDEPKG_NDEBUG
  MSFT:RELEASE_*_*_CC_FLAGS    = /D MDEPKG_NDEBUG
