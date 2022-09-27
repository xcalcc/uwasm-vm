
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include "gtest/gtest.h"
#include "uvm/uvm.h"
#include "runtime/exec_system.h"

static int open_and_mmap(const char *file_path, int *fd, char **buffer, size_t *size) {
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
  *buffer = (char *) mmap(0, fs.st_size, (PROT_READ|PROT_WRITE), MAP_PRIVATE, *fd, 0);
  if ((*buffer) == MAP_FAILED) {
    printf("mmap file failed, file: %s\n", file_path);
    return -1;
  }
  return 0;
}

void test_param(EXPORT_FUNC *funcs, uint32_t idx, uint32_t rt, uint32_t r1, uint32_t r2, uint32_t r3, uint32_t r4) {
  uint32_t real = funcs[idx].param_count;
  uint32_t type_idx = ((UWASM_SYSTEM *) Uvm_system())->Machine().Get_u_module().Get_func(idx).Get_type_idx();
  real = ((UWASM_SYSTEM *) Uvm_system())->Machine().Get_u_module().Get_param_num(type_idx);
  EXPECT_EQ(real, rt) << "Func idx: "<< idx << endl;
  real = ((UWASM_SYSTEM *) Uvm_system())->Machine().Get_u_module().Get_func_param_num<I32>(idx);
  EXPECT_EQ(real, r1) << "Func idx: "<< idx << endl;
  real = ((UWASM_SYSTEM *) Uvm_system())->Machine().Get_u_module().Get_func_param_num<I64>(idx);
  EXPECT_EQ(real, r2) << "Func idx: "<< idx << endl;
  real = ((UWASM_SYSTEM *) Uvm_system())->Machine().Get_u_module().Get_func_param_num<F32>(idx);
  EXPECT_EQ(real, r3) << "Func idx: "<< idx << endl;
  real = ((UWASM_SYSTEM *) Uvm_system())->Machine().Get_u_module().Get_func_param_num<F64>(idx);
  EXPECT_EQ(real, r4) << "Func idx: "<< idx << endl;
}

static void test_fib() {
  const char *file_path = "../tests/as/cgfib/cgfib.uwm";
  int fd;
  char *buffer;
  size_t size;
  int ret = open_and_mmap(file_path, &fd, &buffer, &size);
  if (ret != 0) {
    printf("open and mmap failed.");
    return;
  }
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

static void test_tfjs() {
  const char *file_path = "../tests/as/tfjs/tfjs_backend_uwasm.uwm";
  int fd;
  char *buffer;
  size_t size;
  int ret = open_and_mmap(file_path, &fd, &buffer, &size);
  if (ret != 0) {
    printf("open and mmap failed.");
    return;
  }
  MODULE_HANDLE mh = UVM_load(buffer, size);
  uint32_t mem_size = 1024 * 1024 * 128;
  char *mem_p = (char *) malloc(mem_size);
  UVM_init_memory(mh, mem_p, mem_size);
  EXPORT_FUNC *funcs;
  uint32_t func_number;
  UVM_get_export_funcs(mh, &funcs, &func_number);
  for (int i = 0; i < func_number; i++) {
    char *func_name_ptr = buffer + funcs[i].name_offset;
    printf("Func idx: %2d, name: %s, ret number: %d, parm number: %d\n",
      funcs[i].func_index, func_name_ptr, funcs[i].return_count, funcs[i].param_count);
  }
  UVM_set_func(mh, 0);
  UVM_execute();
  printf("Return value: %d\n", UVM_get_reg_i32(0));
  UVM_free_module(mh);
  UVM_destroy();
  close(fd);
}

namespace {
  TEST(UVM_API_C_TEST, FIB) {
    test_fib();
  }

  TEST(UVM_API_C_TEST, TFJS) {
    test_fib();
  }
}

