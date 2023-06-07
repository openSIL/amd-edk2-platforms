/** @file
  AMD implementation of interface functions for EFI_HII_CONFIG_ROUTING_PROTOCOL.
  This module overrides BlockToConfig and ConfigToBlock for the better performance.

  Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.

**/

#include "AmdHiiConfigRouting.h"

EFI_STATUS
EFIAPI
AmdConfigRoutingEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                       Status;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting = NULL;

  Status = gBS->LocateProtocol (
                  &gEfiHiiConfigRoutingProtocolGuid,
                  NULL,
                  (VOID **)&HiiConfigRouting
                  );
  if (!EFI_ERROR (Status)) {
    ASSERT (HiiConfigRouting != NULL);
    DEBUG ((
      DEBUG_INFO,
      "HiiConfigRouting->BlockToConfig: 0x%lX\n",
      (UINTN)HiiBlockToConfig
      ));
    DEBUG ((
      DEBUG_INFO,
      "HiiConfigRouting->ConfigToBlock: 0x%lX\n",
      (UINTN)HiiConfigToBlock
      ));

    HiiConfigRouting->BlockToConfig = HiiBlockToConfig;
    HiiConfigRouting->ConfigToBlock = HiiConfigToBlock;
  }

  return Status;
}
