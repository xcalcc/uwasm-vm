/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef _UVM_H_
#define _UVM_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum UVM_VALTY {
  UVM_TY_I32 = 0,
  UVM_TY_I64 = 1,
  UVM_TY_F32 = 2,
  UVM_TY_F64 = 3,
};

enum ERROR_NUM {
  NUM_SUCC = 0,
  ERR_NOT_ENOUGH_MEM_BUF,
};

typedef enum UVM_VALTY UVM_VALTY_ENUM;

typedef void * MODULE_HANDLE;
typedef struct {
  uint32_t name_offset;
  uint32_t name_size;
  uint32_t func_index;     // index into the function table.
  uint32_t type_index;     // index into the type table.
  uint16_t param_count;
  uint16_t return_count;
} EXPORT_FUNC;

bool UVM_initialize();
MODULE_HANDLE UVM_load(char *buffer, uint32_t buffer_len);
uint32_t UVM_init_memory(MODULE_HANDLE mh, char *mem_p, uint32_t mem_size);
bool UVM_get_export_funcs(MODULE_HANDLE mh, EXPORT_FUNC **export_funcs, uint32_t *number);
const char *UVM_get_func_name(MODULE_HANDLE mh, uint32_t name_offset); // Deprecated ...
UVM_VALTY_ENUM UVM_get_param_type(MODULE_HANDLE mh, uint32_t func_index, uint16_t param_index);
UVM_VALTY_ENUM UVM_get_return_type(MODULE_HANDLE mh, uint32_t func_index, uint16_t index);
void UVM_set_func(MODULE_HANDLE mh, uint32_t func_idx);
void UVM_set_reg_i32(uint32_t reg, int32_t v);
void UVM_set_reg_i64(uint32_t reg, int64_t v);
void UVM_set_reg_f32(uint32_t reg, float v);
void UVM_set_reg_f64(uint32_t reg, double v);
uint32_t UVM_get_reg_i32(uint32_t reg);
uint64_t UVM_get_reg_i64(uint32_t reg);
float UVM_get_reg_f32(uint32_t reg);
double UVM_get_reg_f64(uint32_t reg);
void UVM_execute();
void UVM_free_module(MODULE_HANDLE mh);
void UVM_destroy();

#ifdef __cplusplus
}
#endif

#endif //UVM_H
