/** @file
  Header file of AMD SMM SPI host controller state protocol

  Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.

**/
#ifndef AMD_SPI_HC_CHIP_SELECT_PARAMETERS_H_
#define AMD_SPI_HC_CHIP_SELECT_PARAMETERS_H_

#include <Base.h>

#pragma pack (1)
typedef struct _CHIP_SELECT_PARAMETERS {
  UINT8   AndValue;
  UINT8   OrValue;
} CHIP_SELECT_PARAMETERS;
#pragma pack ()

CONST CHIP_SELECT_PARAMETERS ChipSelect1 = { (UINT8)~((UINT8)0x03), 0x0 }; // SPI_CS1_L
CONST CHIP_SELECT_PARAMETERS ChipSelect2 = { (UINT8)~((UINT8)0x03), 0x1 }; // SPI_CS1_L

#endif  // AMD_SPI_HC_CHIP_SELECT_PARAMETERS_H_
