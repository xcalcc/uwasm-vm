/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by lishijie on 11/11/20.
//

#include "u_type_encoder.h"

UINT32 U_TYPE_ENCODER::Add_type(vector<VALTYPE> &ret_type_list, vector<VALTYPE> &param_type_list) {
  UINT32 ret_num = ret_type_list.size();
  UINT32 param_num = param_type_list.size();
  if (ret_num + param_num < MAX_TYPE_ELEM_NUM) {
    UINT32 elem = Type_to_number(ret_type_list, param_type_list);
    UBH_TYPE type(ret_num, param_num, elem);
    // determine 2 type are same type or not
    for (UINT32 i = 0; i < _type_list.size(); i++) {
      if (_type_list[i] == type) return i;
    }
    _type_list.push_back(type);
  } else {
    UINT32 byte_num = (ret_num + param_num) * 2 / 8;
    if (((ret_num + param_num) * 2) % 8 != 0) {
      byte_num++;
    }
    UINT32 buf_size = byte_num + 2;
    char *buf = (char *) malloc(buf_size);
    memset(buf, 0, buf_size);
    *((UINT16 *) buf) = param_num;
    for (UINT32 i = 0; i < ret_num; i++) {
      buf[2] |= (ret_type_list[i] & 0x03) << (8 - (i + 1) * 2);
    }
    for (UINT32 i = ret_num; i < ret_num + param_num; i++) {
      UINT32 buf_idx = i / 4;
      char bi = i % 4;
      buf[2 + buf_idx] |= ((UINT8) param_type_list[i - ret_num] & 0x03) << (8 - (bi + 1) * 2);
    }
    UINT32 idx = Nn_tab().Request(buf, buf_size);
    UBH_TYPE type(ret_num, 0xff, Nn_tab().Get_off(idx));
    for (UINT32 i = 0; i < _type_list.size(); i++) {
      if (_type_list[i] == type) return i;
    }
    _type_list.push_back(type);
  }
  return _type_list.size() - 1;
}
