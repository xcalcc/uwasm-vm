/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef U_AM_H
#define U_AM_H

// uwasm valtype definition
typedef enum {
  I32,
  I64,
  F32,
  F64,
  VALTY_MIN = I32,
  VALTY_MAX = F64,
  VALTY_COUNT = F64 + 1, // Use valty_count as for (i = 0; i < VALTY_COUNT; ++i) { ... }
  VALTY_UNKNOWN = 10,
} VALTYPE;

#define VALTY_SZ       2u

// Address Mode type
typedef enum {
  TY_AM32,
  TY_AM32_OFS,
  TY_AM32_R,
  TY_AM32_R_IMM,
  TY_AM32_R_OFS,
  TY_AM32_R_R,
  TY_AM32S_R_R_IMM,
  TY_AM32S_R_R_OFS,
  TY_AM32S_R_R_R,

  TY_AM64_R_IMM,
  TY_AM64_R_OFS,
  TY_AM64_R_R_IMM,
  TY_AM64_R_R_OFS,
  TY_AM64_R_R_R ,
} TY_AM;

#endif //U_AM_H
