## @file
#
#  Copyright (C) 2022-2025 Advanced Micro Devices, Inc. All rights reserved.<BR>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

# CPM patch
!include AmdCpmPkg/Addendum/Oem/$(PLATFORM_CRB)/Processor/$(AMD_PROCESSOR)/AmdCpm$(AMD_PROCESSOR)$(PLATFORM_CRB)Pkg.inc.dsc

# AMD AGESA Include Path
!include $(PROCESSOR_PATH)/Include/AgesaInc/AgesaExt.inc.dsc
