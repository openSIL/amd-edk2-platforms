## @file
#
#  CRB specific - Internal AGESA build.
#  
#  Copyright (C) 2022-2025 Advanced Micro Devices, Inc. All rights reserved.<BR>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##
  #
  # AMD AGESA Includes - Internal
  #
# OPENSIL_OVERRIDE STARTS
!if $(OPENSILSUPPORT) == TRUE
  !include AgesaRedirects/AgesaSp5BrhModulePkg.inc.dsc
  !include AmdOpenSilPkg/opensil-uefi-interface/AmdOpenSilPkgBrh.inc.dsc
!else
  !include AgesaModulePkg/AgesaSp5BrhModulePkg.inc.dsc
!endif
# OPENSIL_OVERRIDE ENDS
  !include AgesaModulePkg/AgesaIdsIntBrh.inc.dsc
