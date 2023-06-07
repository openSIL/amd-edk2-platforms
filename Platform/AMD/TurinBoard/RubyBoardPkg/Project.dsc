## @file
#
#  Copyright (C) 2022-2025 Advanced Micro Devices, Inc. All rights reserved.<BR>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

# *****************************************************************************
# Defines passed into build
# RELEASE_DATE
# FIRMWARE_REVISION_NUM
# FIRMWARE_VERSION_STR
# PLATFORM_CRB
# AMD_PROCESSOR
# *****************************************************************************

[Defines]
!ifndef AMD_PROCESSOR
  AMD_PROCESSOR                  = Turin
!endif
  PROCESSOR_PATH                 = $(AMD_PROCESSOR)Board
!ifndef PLATFORM_CRB
  PLATFORM_CRB                   = Ruby
!endif
  PLATFORM_NAME                  = $(PLATFORM_CRB)BoardPkg
  PLATFORM_GUID                  = C3851035-490E-485E-8941-DFFDBDB45F69
  PLATFORM_VERSION               = 0.01
  DSC_SPECIFICATION              = 1.30
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)_$(AMD_PROCESSOR)
  OUTPUT_DIRECTORY               = $(OUTPUT_DIRECTORY)_EXTERNAL
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = $(PLATFORM_NAME)/Project.fdf

  DEFINE  PEI_ARCH               = IA32
  DEFINE  DXE_ARCH               = X64
  PREBUILD = "python PlatformTools/Server/support/prepostbuild_launcher.py prebuild"
  POSTBUILD = "python PlatformTools/Server/support/prepostbuild_launcher.py postbuild"

  #
  # Platform On/Off features are defined here
  #
  DEFINE SOURCE_DEBUG_ENABLE   = FALSE
  DEFINE DEBUG_DISPATCH_ENABLE = FALSE
  DEFINE DISABLE_SMT           = FALSE

  # AGESA Defines to skip Cf9Reset Driver
  DEFINE AMD_RESET_DXE_DRIVER_SUPPORT_DISABLED  = TRUE

  DEFINE PLATFORM_CRB_TABLE_ID = "RUBY    "

  DEFINE SATA_OVERRIDE         = FALSE

  # AGESA debug output
  DEFINE IDS_DEBUG_ENABLE      = FALSE
  # Non-runtime UEFI output
  DEFINE LOGGING_ENABLE        = FALSE
  # SMM and Dxe runtime debug message control
  DEFINE RUNTIME_LOGGING_ENABLE = FALSE

  # Predefined Fabric Resource
  DEFINE PREDEFINED_FABRIC_RESOURCES = TRUE

  # Multisegment support
  DEFINE PCIE_MULTI_SEGMENT = TRUE

  # EDK2 components are starting to use PLATFORMX64_ENABLE in their include
  # DSC/FDF files
  DEFINE PLATFORMX64_ENABLE    = TRUE

  # MACRO used by AGESA FCH include DSC/FDF to exclude legacy CSM support
  DEFINE AMD_CSM_SUPPORT_DISABLED = TRUE

  DEFINE ROM3_1TB_REMAP = FALSE

  # Console settings
  #
  # Background info:
  #   As per Turin PPR vol7 17.4.10 UART Registers
  #   There are 3 physical UARTS available for SBIOS.
  #   UART0 supports flow controls.
  #   UART1 doest support flow controls.
  #   UART2 is disabled by AGESA/CPM to enable flow control for UART0.
  #   Hence only two UARTs (UART0 and UART1) are available for SBIOS.
  #   MMIO addresses for 4 UART as FEDCF000,FEDCE000,FEDCA000 and FEDC9000
  #
  # Platform settings:
  #   AGESA/CPM enables UART0 and UART1 by setting BIT11 and BIT12 of FchRTDeviceEnableMap.
  #   If SBIOS wants to use MMIO space then above mentioned reserved MMIO can be used.
  #   If SBIOS wants UART in legacy mode(to use 0x3F8/0x2F8) then need to set below PCD for
  #     for respective UART.
  #     FchUart0LegacyEnable, FchUart1LegacyEnable and FchUart2LegacyEnable
  #
  # SERIAL_PORT Options:
  #   NONE
  #   FCH_MMIO    UART0, MMIO
  #   FCH_IO      UART0, 0x3F8
  #   BMC_SOL     UART1, MMIO
  #   BMC_SOL_IO  UART1, 0x3F8
  #   BMC_ESPI    eSPI0, 0x3F8
  DEFINE SERIAL_PORT = "BMC_SOL_IO"
  DEFINE ESPI_UART   = FALSE         # Define ESPI_UART to modify APCB tokens

  DEFINE USB_SUPPORT           = TRUE
  DEFINE SATA_SUPPORT          = TRUE
  DEFINE NVME_SUPPORT          = TRUE

  #
  # Check undefined variables
  #
!ifndef RELEASE_DATE
  RELEASE_DATE                   = 01/01/2023
!endif
!ifndef FIRMWARE_VERSION_STR
  FIRMWARE_VERSION_STR           = NONE
!endif
!ifndef FIRMWARE_REVISION_NUM
  FIRMWARE_REVISION_NUM          = 0x00000000
!endif

#-----------------------------------------------------------
#  End of [Defines] section
#-----------------------------------------------------------

# Add platform includes AGESA, CPM etc
!include $(PROCESSOR_PATH)/Include/Dsc/Platform.inc.dsc

# Board specific SMBIOS defines
!include $(PLATFORM_NAME)/Include/Dsc/Smbios.dsc

# Platform Common PCDs
!include $(PROCESSOR_PATH)/Include/Dsc/PlatformCommonPcd.dsc.inc

# Board specific PCDs
[PcdsFixedAtBuild]
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSmbiosSocketDesignationSocket0|"P0"
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x2020202059425552  # "RUBY    "
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuCoreCount|384
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuSocketCount|1
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|384

[PcdsDynamicDefault]
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdEarlyBmcLinkLaneNum|134
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXhciOcPolarityCfgLow|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXhciUsb31OcPinSelect|0xFFFF1010
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXhciUsb20OcPinSelect|0xFFFFFFFFFFFF1010
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdCfgPlatformPPT|400

#######################################
# Library Includes
#######################################
!include MinPlatformPkg/Include/Dsc/CoreCommonLib.dsc
!include MinPlatformPkg/Include/Dsc/CorePeiLib.dsc
!include MinPlatformPkg/Include/Dsc/CoreDxeLib.dsc
# do not change the order of include
!include $(PROCESSOR_PATH)/Include/Dsc/ProjectCommon.inc.dsc
