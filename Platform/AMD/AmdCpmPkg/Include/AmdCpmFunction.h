/** @file

  AMD CPM Common Definitions.
  
  Copyright (C) 2023-2025 Advanced Micro Devices, Inc. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef AMD_CPM_FUNCTION_H_
#define AMD_CPM_FUNCTION_H_

typedef VOID *(EFIAPI *AMD_CPM_GETTABLEPTR_FN)(
  IN       VOID                        *This,
  IN       UINT32                      TableId
  );

typedef AGESA_STATUS (EFIAPI *AMD_CPM_GETPOSTEDVBIOSIMAGE_FN)(
  IN      GFX_VBIOS_IMAGE_INFO         *VbiosImageInfo
  );

/// Common Functions for CPM Drivers
typedef struct _AMD_CPM_COMMON_FUNCTION {
  AMD_CPM_GETTABLEPTR_FN    GetTablePtr2;                   ///< Get CPM Table Pointer. The table can be re-writable
} AMD_CPM_COMMON_FUNCTION;

#endif //AMD_CPM_FUNCTION_H_
