/** @file

  Copyright (C) 2023-2025 Advanced Micro Devices, Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _SMNACCESSLIB_H_
#define _SMNACCESSLIB_H_

VOID
SmnRegisterReadS (
  IN       UINT32              SegmentNumber,
  IN       UINT32              BusNumber,
  IN       UINT32              Address,
  OUT      VOID                *Value
  );

VOID
SmnRegisterRMWS (
  IN       UINT32              SegmentNumber,
  IN       UINT32              BusNumber,
  IN       UINT32              Address,
  IN       UINT32              AndMask,
  IN       UINT32              OrValue,
  IN       UINT32              Flags
  );
#endif


