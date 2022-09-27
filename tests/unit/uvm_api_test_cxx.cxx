//
// Created by lishijie on 9/11/20.
//
#if 0
#include "uvm/uvm.h"
#include "../../src/runtime/exec_system.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#if 0
void Quit_with_tracing(const char * file, uint32_t line, const char *inst_impl_func_name) {
  fprintf(stderr, "\n\n\n### In file %s:%d, func: %s\n", file, line, inst_impl_func_name);
}

void Assertion_Failure_Print ( const char *fmt, ... )
{
  va_list vp;

  char msg[512];
  int32_t pos;

  /* Prepare header line: */
  va_start(vp, fmt);
  pos = sprintf ( &msg[0], "### Assertion Failure : \n");
  pos += vsprintf ( &msg[pos], fmt, vp );
  sprintf ( &msg[pos], "\n" );
  va_end ( vp );
  fprintf(stderr, "%s\n", msg);
  exit(1);
}
#endif
#ifdef Is_True
#undef Is_True
#endif
#define Is_True(Condition, Params) \
        (Condition ? (void) 1 \
        :(Quit_with_tracing ( __FILE__, __LINE__, __func__),	\
        Assertion_Failure_Print Params) );

int open_and_mmap(char *file_path, int *fd, char **buffer, size_t *size) {
  struct stat fs;
  *fd = open(file_path, O_RDONLY | O_CLOEXEC);
  if (*fd < 0) {
    printf("Open file error, file: %s.\n", file_path);
    return -1;
  }
  if (fstat(*fd, &fs) < 0) {
    printf("Read file state failed, file: %s.\n", file_path);
    return -1;
  }
  *size = fs.st_size;
  *buffer = (char *) mmap(0, fs.st_size, PROT_READ, MAP_SHARED, *fd, 0);
  if ((*buffer) == MAP_FAILED) {
    printf("mmap file failed, file: %s\n", file_path);
    return -1;
  }
  return 0;
}

void test_fib() {
  const char *file_path = "../tests/as/cgfib/cgfib.uwm";
  int fd;
  char *buffer;
  size_t size;
  int ret = open_and_mmap((char*)file_path, &fd, &buffer, &size);
  if (ret != 0) {
    printf("open and mmap failed.");
    return;
  }
  UVM_initialize();
  MODULE_HANDLE mh = UVM_load(buffer, size);
  uint32_t mem_size = 1024 * 1024 * 128;
  char *mem_p = (char *) malloc(mem_size);
  UVM_init_memory(mh, mem_p, mem_size);
  EXPORT_FUNC *funcs;
  uint32_t func_number;
  UVM_get_export_funcs(mh, &funcs, &func_number);
  for (int i = 0; i < func_number; i++) {
    char *func_name_ptr = buffer + funcs[i].name_offset;
    printf("Func idx: %2d, name: %s\n", funcs[i].func_index, func_name_ptr);
  }
  UVM_set_func(mh, 0);
  UVM_execute();
  printf("Return value: %d\n", UVM_get_reg_i32(0));
  UVM_free_module(mh);
  UVM_destroy();
  close(fd);
}

void test_param(EXPORT_FUNC *funcs, uint32_t idx, uint32_t rt, uint32_t r1, uint32_t r2, uint32_t r3, uint32_t r4) {
  uint32_t real = funcs[idx].param_count;
  Is_True(real == rt,
    ("Func idx: %d, required param count: %d, real count: %d.", idx, rt, real));
  uint32_t type_idx = Uvm_system()->Machine().Get_u_module().Get_func(idx).Get_type_idx();
  real = Uvm_system()->Machine().Get_u_module().Get_param_num(type_idx);
  Is_True(real == rt,
          ("Func idx: %d, required parameter count: %d, real count: %d.", idx, rt, real));
  real = Uvm_system()->Machine().Get_u_module().Get_func_param_num<I32>(idx);
  Is_True(real == r1,
          ("Func idx: %d, int32 type, required param count: %d, real count: %d.", idx, r1, real));
  real = Uvm_system()->Machine().Get_u_module().Get_func_param_num<I64>(idx);
  Is_True(real == r2,
          ("Func idx: %d, int64 type, required param count: %d, real count: %d.", idx, r2, real));
  real = Uvm_system()->Machine().Get_u_module().Get_func_param_num<F32>(idx);
  Is_True(real == r3,
          ("Func idx: %d, float32 type, required param count: %d, real count: %d.", idx, r3, real));
  real = Uvm_system()->Machine().Get_u_module().Get_func_param_num<F64>(idx);
  Is_True(real == r4,
          ("Func idx: %d, float64 type, required param count: %d, real count: %d.", idx, r4, real));
}

void test_tfjs_backend() {
  const char *file_path = "../tests/binary/tfjs_backend/tfjs_backend_uwasm.uwm";
  int fd;
  char *buffer;
  size_t size;
  int ret = open_and_mmap((char*)file_path, &fd, &buffer, &size);
  if (ret != 0) {
    printf("open and mmap failed.");
    return;
  }
  UVM_initialize();
  MODULE_HANDLE mh = UVM_load(buffer, size);
  uint32_t mem_size = 1024 * 1024 * 128;
  char *mem_p = (char *) malloc(mem_size);
  UVM_init_memory(mh, mem_p, mem_size);
  EXPORT_FUNC *funcs;
  uint32_t func_number;
  UVM_get_export_funcs(mh, &funcs, &func_number);
  for (int i = 0; i < func_number; i++) {
    char *func_name_ptr = buffer + funcs[i].name_offset;
    uint32_t func_index = funcs[i].func_index;
    printf("Func idx: %2d, name: %s, ret number: %d, parm number: %d, i32 parm: %d, i64 parm: %d, f32 parm: %d, f64 parm: %d\n",
      func_index, func_name_ptr, funcs[i].return_count, funcs[i].param_count,
      Uvm_system()->Machine().Get_u_module().Get_func_param_num<I32>(func_index),
      Uvm_system()->Machine().Get_u_module().Get_func_param_num<I64>(func_index),
      Uvm_system()->Machine().Get_u_module().Get_func_param_num<F32>(func_index),
      Uvm_system()->Machine().Get_u_module().Get_func_param_num<F64>(func_index));
  }
  // $register_tensor (type $tregister_tensor) (param $r2 i32) (param $r3 i32) (param $r4 i32)
  test_param(funcs, 2, 3, 3, 0, 0, 0);
  // $Conv2D (type $tConv2D) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $r10 i32) (param $r11 i32) (param $r12 i32) (param $r13 i32) (param $r14 i32) (param $r15 i32) (param $r16 i32) (param $r17 i32) (param $r18 i32) (param $r19 i32) (param $r20 i32)
  test_param(funcs, 8, 19, 19, 0, 0, 0);
  // $PadV2 (type $tPadV2) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $f2 f32) (param $r8 i32)
  test_param(funcs, 28, 8, 7, 0, 1, 0);
  // $Conv2DBackpropInput (type $tConv2DBackpropInput) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $r10 i32) (param $r11 i32) (param $r12 i32) (param $r13 i32) (param $r14 i32) (param $r15 i32) (param $r16 i32) (param $r17 i32) (param $r18 i32) (param $r19 i32) (param $r20 i32) (param $r21 i32) (param $r22 i32) (param $r23 i32) (param $r24 i32) (param $r25 i32) (param $r26 i32) (param $r27 i32) (param $r28 i32)
  test_param(funcs, 56, 27, 27, 0, 0, 0);
  UVM_set_func(mh, 3);
  UVM_set_reg_i32(2, 1);
  UVM_set_reg_i32(3, 2);
  UVM_set_reg_i32(4, 3);
  UVM_execute();
  printf("Return value: %d\n", UVM_get_reg_i32(0));
  UVM_free_module(mh);
  UVM_destroy();
  close(fd);
}

int main(int argc, char **argv) {
  test_fib();
  test_tfjs_backend();
}
#endif