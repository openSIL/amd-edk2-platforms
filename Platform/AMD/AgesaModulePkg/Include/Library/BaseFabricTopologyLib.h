/** @file

  Copyright (C) 2023-2025 Advanced Micro Devices, Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef BASE_FABRIC_TOPOLOGY_LIB_H_
#define BASE_FABRIC_TOPOLOGY_LIB_H_

UINTN
FabricTopologyGetNumberOfProcessorsPresent (
  VOID
  );


UINTN
FabricTopologyGetNumberOfRootBridgesOnSocket (
  IN       UINTN Socket
  );


#endif // BASE_FABRIC_TOPOLOGY_LIB_H_
