/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef UWASM_VM_U_TYPE_ENCODER_H
#define UWASM_VM_U_TYPE_ENCODER_H

#include <vector>
#include "ubh/ubh.h"
#include "utilities/u_str_tab.h"

using namespace std;

class U_TYPE_ENCODER {
private:
  vector<UBH_TYPE>            _type_list;
  U_STR_TAB                  &_nn_tab;

private:
  UINT32                      Type_to_number(vector<VALTYPE> &ret_type_list, vector<VALTYPE> &param_type_list);

public:
  U_TYPE_ENCODER(U_STR_TAB &nn_tab) : _nn_tab(nn_tab)                     {}
  UINT32                      Add_type(vector<VALTYPE> &ret_type_list, vector<VALTYPE> &param_type_list);
  vector<UBH_TYPE>           &Get_type_list()                           { return _type_list; }
  U_STR_TAB                  &Nn_tab()                                  { return _nn_tab; }
};

#endif //UWASM_VM_U_TYPE_ENCODER_H
