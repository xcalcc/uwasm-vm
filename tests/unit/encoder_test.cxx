//
// Created by xc5 on 2020/8/30.
//

#include "utilities/u_utils.h"
#include "ubh/ubh.h"
#include "encoder/u_encoder.h"
#include "trace/u_trace.h"
#include "gtest/gtest.h"

namespace {
  TEST(U_ENCODER_TEST, TEST1) {
    // global variable start from 1
    U_ENCODER encoder;
    UINT32 g1 = encoder.Add_global("abc");
    EXPECT_EQ(g1, 1);
    UINT32 g2 = encoder.Add_global("bcdef");
    EXPECT_EQ(g2, 2);
    vector<VALTYPE> ret_list, param_list;
    UINT32 locals[4], params[4];
    for (UINT32 i = 0; i < VALTY_COUNT; i++) {
      ret_list.push_back((VALTYPE) 0);
      param_list.push_back((VALTYPE) 0);
      locals[i] = 0;
      params[i] = 0;
    }
    UINT32 ty_idx = encoder.Add_type(ret_list, param_list);
    UINT32 flags = 0;
    UINT32 func_idx = encoder.Request_func("some_external_func", ty_idx, flags);
    encoder.Start_func(func_idx, locals, params, 0 /* stack size */);
  }
}
