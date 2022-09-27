/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by lishijie on 10/21/20.
//

const char *inst_impl_func_name[] = {
#define EXEC_FUNC(func_name) #func_name,
#define DUP_FUNC(func_name) #func_name,
#define UNUSED() "unused",
#include "interpreter/u_func_list.def"

#undef EXEC_FUNC
#undef DUP_FUNC
#undef UNUSED
};
