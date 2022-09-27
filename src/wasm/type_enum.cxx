/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by Stephen Ark on 8/4/2020.
//
#include "type_enum.h"
#include "utilities/u_utils.h"
#include "trace/u_trace.h"

VALTYPE Conv_wabt_ty(UINT32 wabt_enum) {
  for (UINT32 i = 0;
       i < sizeof(wabt_op_tab) / sizeof(WABT_to_OPTYPE_info); i++) {
    if (wabt_op_tab[i].wabttype == (WABT_ENUM) wabt_enum) {
      return wabt_op_tab[i].optype;
    }
  }
  Is_True(false, ("We cannot have enum %d converted to optype, not in table.", wabt_enum));
  return VALTYPE::I32;
}

WABT_ENUM Conv_ty_wabt(UINT32 valty) {
  for (UINT32 i = 0;
       i < sizeof(wabt_op_tab) / sizeof(WABT_to_OPTYPE_info); i++) {
    if (wabt_op_tab[i].optype == (VALTYPE) valty) {
      return wabt_op_tab[i].wabttype;
    }
  }
  Is_True(false, ("We cannot have enum %d converted to wabt::Type, not in table.", valty));
  return WABT_ENUM::WABT_Void;
}