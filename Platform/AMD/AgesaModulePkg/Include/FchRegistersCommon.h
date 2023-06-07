/** @file

  Copyright (C) 2023-2025 Advanced Micro Devices, Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef FCH_REGISTER_COMMON_H_
#define FCH_REGISTER_COMMON_H_

#define FCH_LPC_BUS   0
#define FCH_LPC_DEV   20
#define FCH_LPC_FUNC  3

#define ACPI_MMIO_BASE  0xFED80000ul
#define SMI_BASE        0x200           // DWORD
#define IOMUX_BASE      0xD00           // BYTE

//
//  FCH LPC Device  0x780E
//  Device 20 (0x14) Func 3
//
#define FCH_LPC_REG48  0x48             // IO/Mem Port Decode Enable Register 5- RW
#define FCH_LPC_REG74  0x74             // Alternative Wide IO Range Enable- W/R
#define FCH_LPC_REG7C  0x7C             // TPM (trusted plant form module) reg- W/R
#define FCH_LPC_REGA0  0x0A0            // SPI base address
#define FCH_LPC_REGB8            0x0B8

//
// FCH SPI
//

#define FCH_SPI_BASE_ADDRESS  0xFEC10000

#define FCH_SPI_MMIO_REG00  0x00
#define FCH_SPI_FIFO_PTR_CRL     0x00100000l  //
#define FCH_SPI_BUSY             0x80000000l  //
#define FCH_SPI_MMIO_REG1D       0x1D        //
#define FCH_SPI_MMIO_REG20  0x20
#define FCH_SPI_MMIO_REG22       0x22        //
#define FCH_SPI_MMIO_REG45_CMDCODE           0x45        //
#define FCH_SPI_MMIO_REG47_CMDTRIGGER        0x47        //
#define FCH_SPI_MMIO_REG48_TXBYTECOUNT       0x48        //
#define FCH_SPI_MMIO_REG4B_RXBYTECOUNT       0x4B        //
#define FCH_SPI_MMIO_REG4C_SPISTATUS         0x4C        //
#define FCH_SPI_MMIO_REG80_FIFO              0x80        //

#endif /* FCH_REGISTER_COMMON_H_ */
