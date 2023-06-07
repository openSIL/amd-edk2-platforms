/** @file

  Copyright (C) 2023-2025 Advanced Micro Devices, Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>

UINTN
FabricTopologyGetNumberOfProcessorsPresent (
  VOID
  )
{
  return 0;
}

UINTN
FabricTopologyGetNumberOfRootBridgesOnSocket (
  IN     UINTN Socket
  )
{

  return 0;
}
