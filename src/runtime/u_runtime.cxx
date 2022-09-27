/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include <random>
#include <dlfcn.h>
#include <encoder/u_encoder.h>
#include "u_runtime.h"
#include "u_runtime_reg.h"
#include "interpreter/exec_unit.h"
#include "sys/uio.h"

#define SYSCALL_PREFIX "[SYSCALL] "
#define MAX_PARAM_NUM 256
#define RUNTIME_ZERO_REG 0
#define NANOS_PER_SEC 1000000000

// Define register index behavior
#ifndef UWASM_VM_RUNTIME_WASM
// uWASM native mode
#define RUNTIME_PARM_REG(x)  ((x + 1))
#define RUNTIME_Z_BASE_REG(x)  ((x))
#else
// wasm compatible mode.
#define RUNTIME_PARM_REG(x)  ((x + RET_REG_NUM + CONST_REG_NUM))
#define RUNTIME_Z_BASE_REG(x)  ((x + RET_REG_NUM + CONST_REG_NUM))
#endif

typedef UINT32 U_INT32_PTR;

NAME_PTR_MAP U_RUNTIME::_name_ptr_cache;
vector<FUNC_INFO *> U_RUNTIME::_func_infos;
// allocate actual array, used by ffi
static void *ACTUAL_ARR[MAX_PARAM_NUM];
// if a parameter native type is point type, should saved to this array
static void *POINT_PARAM_ARR[MAX_PARAM_NUM];
static void *POINT_RET;

void U_RUNTIME::Print_syscall_parm(UWASM_MACHINE &m, VALTYPE parm_ty_arr[], UINT32 parm_len, FILE *file) {
  UINT32 idx[VALTY_COUNT];
  for (INT32 i = 0; i < VALTY_COUNT; i++) {
    idx[i] = 0;
  }
  fprintf(file, "parameter (");
  for (UINT32 i = 0; i < parm_len; i++) {
    m.Print_reg(parm_ty_arr[i], idx[i]++, file);
    if (i != parm_len - 1) {
      fprintf(file, ", ");
    }
  }
  fprintf(file, ")\n");
}

void U_RUNTIME::Print_syscall_parm(UWASM_MACHINE &m, VALTYPE ret_ty, VALTYPE parm_ty_arr[], UINT32 parm_len, FILE *file) {
  UINT32 idx[VALTY_COUNT];
  for (INT32 i = 0; i < VALTY_COUNT; i++) {
    idx[i] = 0;
  }
  idx[ret_ty]++;
  fprintf(file, "parameter (");
  for (UINT32 i = 0; i < parm_len; i++) {
    m.Print_reg(parm_ty_arr[i], idx[i]++, file);
    if (i != parm_len - 1) {
      fprintf(file, ", ");
    }
  }
  fprintf(file, ")\n");
}

void U_RUNTIME::Print_syscall_ret(UWASM_MACHINE &m, VALTYPE ret_ty, FILE *file) {
  fprintf(file, "return (");
  m.Print_reg(ret_ty, 0, file);
  fprintf(file, ")\n");
}

/*
 *  int putchar(int)
 */
void U_RUNTIME::VM_SYSCALL_putchar(UWASM_MACHINE &m) {
  VALTYPE ret_ty = I32;
  VALTYPE parm_ty_arr[1] = {I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, ret_ty, parm_ty_arr, 1, TFile));
  INT32 charval = m.Reg_i32().Get(RUNTIME_PARM_REG(0));
  Is_Trace(Tracing(TP_EXEC), (TFile, SYSCALL_PREFIX"putchar(%d)\n", charval));
  int ret = putchar(charval);
  m.Reg_i32().Set(0, ret);
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_ret(m, ret_ty, TFile));
}

/*
 *  void uwasm_putlong(long)
 */
void U_RUNTIME::VM_SYSCALL_uwasm_putlong(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[1] = {I64};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 1, TFile));
  INT64 i64_val = m.Reg_i64().Get(RUNTIME_Z_BASE_REG(0));
  Is_Trace(Tracing(TP_EXEC), (TFile, SYSCALL_PREFIX"uwasm_putlong(%lld)\n", i64_val));
  printf("%lld", i64_val);
}

/*
 * void uwasm_putfloat(float)
 */
void U_RUNTIME::VM_SYSCALL_uwasm_putfloat(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[1] = {F32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 1, TFile));
  float f32_val = m.Reg_f32().Get(RUNTIME_Z_BASE_REG(0));
  Is_Trace(Tracing(TP_EXEC), (TFile, SYSCALL_PREFIX"uwasm_putfloat(%f)\n", f32_val));
  printf("%f", f32_val);
}

/*
 * void uwasm_putdouble(double)
 */
void U_RUNTIME::VM_SYSCALL_uwasm_putdouble(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[1] = {F64};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 1, TFile));
  double f64_val = m.Reg_f64().Get(RUNTIME_Z_BASE_REG(0));
  Is_Trace(Tracing(TP_EXEC), (TFile, SYSCALL_PREFIX"uwasm_putfloat(%f)\n", f64_val));
  printf("%lf", f64_val);
}

/*
 * ATTENTION: current implementation is for 32 bit architecture
 * void uwasm_putstr(char *)
 */
void U_RUNTIME::VM_SYSCALL_uwasm_putstr(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[1] = {I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 1, TFile));
  char *str = m.Get_base_addr(RUNTIME_Z_BASE_REG(0));
  Is_Trace(Tracing(TP_EXEC), (TFile, SYSCALL_PREFIX"uwasm_putstr(\"%s\")\n", str));
  printf("%s", str);
}

/*
 * void *malloc(long)
 */
void U_RUNTIME::VM_SYSCALL_malloc(UWASM_MACHINE &m) {
  VALTYPE ret_ty = I32;
#ifdef TARG_32BIT
  VALTYPE parm_ty_arr[1] = {I32};
  long malloc_size = (long) m.Reg_i32().Get(1);
#else
  VALTYPE parm_ty_arr[1] = {I64};
  long malloc_size = (long) m.Reg_i64().Get(RUNTIME_PARM_REG(0));
#endif
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, ret_ty, parm_ty_arr, 1, TFile));
#ifdef DIRECT_ACC_MEM
  // for 32 bit only for now
  char *mem = U_malloc(m, malloc_size) + m.Mem_pointer();
#ifdef TARG_32BIT
  m.Reg_i32().Set(0, (INTPTR)mem);
#else
  m.Reg_i64().Set(0, (INTPTR)mem);
#endif
  Is_Trace(Tracing(TP_EXEC), (TFile, SYSCALL_PREFIX"malloc(%lu): mem(0x%p)\n", malloc_size, mem));
#else
  INT32 mem = U_malloc(m, malloc_size);
  m.Reg_i32().Set(0, mem);
  Is_Trace(Tracing(TP_EXEC), (TFile, SYSCALL_PREFIX"malloc(%lu): offset(%zd)\n", malloc_size, (size_t)mem));
#endif
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_ret(m, ret_ty, TFile));
}

/*
 * void free(void *)
 */
void U_RUNTIME::VM_SYSCALL_free(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[1] = {I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 1, TFile));
#ifdef DIRECT_ACC_MEM
  U_free(m, (char *)m.Reg_i32().Get(0) - m.Mem_pointer());
  Is_Trace(Tracing(TP_EXEC),
           (TFile, SYSCALL_PREFIX"free(): mem(0x%p)\n",
             (char *)m.Reg_i32().Get(0)));
#else
  U_free(m, m.Reg_i32().Get(RUNTIME_Z_BASE_REG(0)));
  Is_Trace(Tracing(TP_EXEC),
           (TFile, SYSCALL_PREFIX"free(): offset(%zd)\n",
             (size_t) m.Reg_i32().Get(RUNTIME_Z_BASE_REG(0))));
#endif
}

void U_RUNTIME::VM_SYSCALL_one(UWASM_MACHINE &m) {
  // Prologue here.
  // Do what ever needed.
  m.Reg_i32().Set(0, 1);
}

void U_RUNTIME::VM_SYSCALL_zero(UWASM_MACHINE &m) {
  // Do what ever needed.
  m.Reg_i32().Set(0, 0);
}

void U_RUNTIME::VM_SYSCALL_args_sizes_get(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[2] = {I32, I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 2, TFile));
  INT32 *nargs     = (INT32 *) m.Get_base_addr(RUNTIME_Z_BASE_REG(0));
  INT32 *data_size = (INT32 *) m.Get_base_addr(RUNTIME_Z_BASE_REG(1));
  Is_Trace(Tracing(TP_EXEC), (TFile, SYSCALL_PREFIX"args_sizes_get(%p, %p)\n", nargs, data_size));
  UINT32 total_size = m.Get_string_args().size();
  UINT32 total_len  = 0;
  for (UINT32 i = 0; i < total_size; i++) {
    string &one_arg = m.Get_string_args().at(i);
    total_len += one_arg.size() + 1; // and a trailing '\0'.
  }
  *nargs = total_size;
  *data_size = total_len;
}

void U_RUNTIME::VM_SYSCALL_args_get(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[2] = {I32, I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 2, TFile));
  U_INT32_PTR *args_ptr  = (U_INT32_PTR *) m.Get_base_addr(RUNTIME_Z_BASE_REG(0));
  U_INT32_PTR base_addr  = m.Reg_i32().Get(RUNTIME_Z_BASE_REG(1));
  char *data_chunk       = (char *) m.Get_base_addr(RUNTIME_Z_BASE_REG(1));
  Is_Trace(Tracing(TP_EXEC), (TFile, SYSCALL_PREFIX"args_get(%p, %u[real: %p])\n", args_ptr, base_addr, data_chunk));
  UINT32 total_size = m.Get_string_args().size();
  UINT32 total_len  = 0;
  for (UINT32 i = 0; i < total_size; i++) {
    string &one_arg = m.Get_string_args().at(i);
    // TODO: Bounds check.
    args_ptr[i] = base_addr + total_len; // set the addr to arguments table;
    memcpy(data_chunk + total_len, one_arg.data(), one_arg.size()); // copy string to buffer.
    *(data_chunk + total_len + one_arg.size()) = '\0'; // write trailing \0
    total_len += one_arg.size() + 1; // update total_len.
  }
}

typedef UINT32 __wasi_size_t;
typedef struct __wasi_ciovec_t {
  /**
   * The address of the buffer to be written.
   */
  UINT32 buf_ptr;

  /**
   * The length of the buffer to be written.
   */
  __wasi_size_t buf_len;

} __wasi_ciovec_t;

void U_RUNTIME::VM_SYSCALL_fd_write(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[4] = {I32, I32, I32, I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 4, TFile));
  const char      *mem_base_addr = m.Mem_pointer();
  // Get the arguments.
  INT32            fd            = m.Reg_i32().Get(RUNTIME_PARM_REG(0));
  __wasi_ciovec_t *iovs          = (__wasi_ciovec_t *) m.Get_base_addr(RUNTIME_PARM_REG(1));
  __wasi_size_t    iovs_len      = m.Reg_i32().Get(RUNTIME_PARM_REG(2));
  __wasi_size_t   *result_addr   = (__wasi_size_t *) m.Get_base_addr(RUNTIME_PARM_REG(3));
  Is_Trace(Tracing(TP_EXEC), (TFile, SYSCALL_PREFIX"fd_write(%d, %p, %u, %p)\n", fd, iovs, iovs_len, result_addr));
  // Copy to real_iovs vector.
  vector<struct iovec> real_iovs; // *real_iovs = new struct iovec[iovs_len];
  real_iovs.resize(iovs_len);
  for (UINT32 i = 0; i < iovs_len; i++) {
    UINT32 ptr = iovs[i].buf_ptr;
    UINT32 buf_len = iovs[i].buf_len;
    Is_True(ptr + buf_len < m.Mem_size(),
            ("Pointer exceed memory bound. buf_ptr = %u, buf_len = %u", ptr, buf_len))
    real_iovs[i].iov_base = (void *) (mem_base_addr + ptr);
    real_iovs[i].iov_len = buf_len;
  }
  size_t res = writev(fd, real_iovs.data(), iovs_len);
  *result_addr = res;
  m.Reg_i32().Set(0, 0);
}

void U_RUNTIME::VM_SYSCALL_uwasm_printfi(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[2] = {I32, I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 2, TFile));
  const char *fmt = m.Get_base_addr(RUNTIME_Z_BASE_REG(0));
  INT32 value = m.Reg_i32().Get(RUNTIME_PARM_REG(1));
  Is_Trace(Tracing(TP_EXEC), (TFile, SYSCALL_PREFIX"uwasm_printfi(\"%s\", %d)\n", fmt, value));
  printf(fmt, value);
}

void U_RUNTIME::VM_SYSCALL_uwasm_scanfv(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[2] = {I32, I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 2, TFile));
  const char *fmt = m.Get_base_addr(RUNTIME_Z_BASE_REG(0));
  const char *valueptr = m.Get_base_addr(RUNTIME_Z_BASE_REG(1));
  Is_Trace(Tracing(TP_EXEC), (TFile, SYSCALL_PREFIX"uwasm_scanfv(\"%s\", %p)\n", fmt, valueptr));
  scanf(fmt, valueptr);
}

void U_RUNTIME::VM_SYSCALL_uwasm_putchar(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[1] = {I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 1, TFile));
  INT32 ch = m.Reg_i32().Get(RUNTIME_PARM_REG(0));
  putchar(ch);
}

void U_RUNTIME::VM_SYSCALL_uwasm_putint(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[1] = {I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 1, TFile));
  INT32 val = m.Reg_i32().Get(RUNTIME_PARM_REG(0));
  printf("%d",val);
}

void U_RUNTIME::VM_SYSCALL_uwasm_randii(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[2] = {I32, I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 2, TFile));
  INT32 min = m.Reg_i32().Get(RUNTIME_PARM_REG(0)), max = m.Reg_i32().Get(RUNTIME_PARM_REG(1));
  std::random_device                  rand_dev;
  std::mt19937                        generator(rand_dev());
  std::uniform_int_distribution<INT32> distribution(min, max);
  m.Reg_i32().Set(0, distribution(generator));
}

void U_RUNTIME::VM_SYSCALL_uwasm_getchar(UWASM_MACHINE &m) {
  char reschar;
  INT32 scanf_cnt = scanf("%c", &reschar);
  Is_True(scanf_cnt == 1, ("Must be 1, yet = %d",scanf_cnt));
  m.Reg_i32().Set(0, (INT32) reschar);
}

void U_RUNTIME::VM_SYSCALL_uwasm_getint(UWASM_MACHINE &m) {
  INT32 reschar = 0;
  INT32 scanf_cnt = scanf("%d", &reschar);
  Is_True(scanf_cnt == 1, ("Must be 1, yet = %d",scanf_cnt));
  m.Reg_i32().Set(0, reschar);
}

void U_RUNTIME::VM_SYSCALL_uwasm_rand(UWASM_MACHINE &m) {
  std::random_device                  rand_dev;
  std::mt19937                        generator(rand_dev());
  std::uniform_int_distribution<INT32> distribution(INT_MIN, INT_MAX);
  m.Reg_i32().Set(0, distribution(generator));
}

void U_RUNTIME::VM_SYSCALL_uwasm_assert_true(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[1] = {I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 1, TFile));
  INT32 given = m.Reg_i32().Get(RUNTIME_PARM_REG(0));
  if (given != 0) {
    m.Print(stderr);
  }
  Is_True_Rel(given != 0, ("[ASSERT] uwasm_assert_true(value), yet value is zero"));
}

void U_RUNTIME::VM_SYSCALL_proc_exit(UWASM_MACHINE &m) {
  // Do what ever needed.
  VALTYPE parm_ty_arr[1] = {I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 1, TFile));
  INT32 exitval = m.Reg_i32().Get(RUNTIME_PARM_REG(0));
  Is_Trace(Tracing(TP_EXEC), (TFile, "[EXIT] Proc_exit invoked = %d\n", exitval));
  exit(exitval);
}

void U_RUNTIME::VM_SYSCALL_trap(UWASM_MACHINE &m) {
  // Do what ever needed.
  VALTYPE parm_ty_arr[1] = {I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 1, TFile));
  fprintf(stderr, "[TRAP] Trap syscall invoked, exit with uvm value = 6, trap-param = %d\n", m.Reg_i32().Get(RUNTIME_PARM_REG(0)));
  exit(6);
}

/*
 * int clock_settime(clockid_t clockid, const struct timespec *tp);
 */
void U_RUNTIME::VM_SYSCALL_clock_gettime(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[2] = {I32, I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 2, TFile));
  INT32 clockid = m.Reg_i32().Get(RUNTIME_PARM_REG(0));
  char *base_addr = m.Get_base_addr(RUNTIME_PARM_REG(1));
#ifdef DIRECT_ACC_MEM
  INT32 ret = clock_gettime((clockid_t) clockid, (timespec*)base_addr);
#else
  timespec  *ptr = (timespec  *)base_addr;
  timespec ts;
  INT32 ret = clock_gettime((clockid_t) clockid, &ts);
  ptr->tv_sec = ts.tv_sec;
#ifdef TARG_32BIT
  // tv_sec(long int) byte size is 4 in 32 bit
  // the nsec may be truncated
  INT32 tv_sec_size = 4;
  *(INT32*)((char *)ptr + tv_sec_size) = ts.tv_nsec;
#else
  ((timespec*)base_addr)->tv_nsec = ts.tv_nsec;
#endif
#endif
  m.Reg_i32().Set(0, ret);
  Is_Trace(Tracing(TP_EXEC),
           (TFile, SYSCALL_PREFIX"clock_gettime(\"%d, %p\") = %d, sec=%ld\n",
            clockid, base_addr, ret, ((timespec*)base_addr)->tv_sec));
}


/*
 * * Return the time value of a clock.
 * Note: This is similar to `clock_gettime` in POSIX.
 * @return
 * The time value of the clock.
 *
  __wasi_errno_t __wasi_clock_time_get(
  **
   * The clock for which to return the time.
   *
  __wasi_clockid_t id,
  **
   * The maximum lag (exclusive) that the returned time value may have, compared to its actual value.
   *
  __wasi_timestamp_t precision,
  __wasi_timestamp_t *retptr0
) __attribute__((__warn_unused_result__));
 */
void U_RUNTIME::VM_SYSCALL_clock_time_get(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[3] = {I32, I64, I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 3, TFile));
  INT32 clockid   = m.Reg_i32().Get(RUNTIME_PARM_REG(0));
  INT64 precision = m.Reg_i64().Get(RUNTIME_PARM_REG(0));
  INT64 *base_addr = (INT64 *) m.Get_base_addr(RUNTIME_PARM_REG(1));
  timespec ts;
  INT32 ret = clock_gettime((clockid_t) clockid, &ts);
  INT64 result = 0;

  switch (clockid) {
    case CLOCK_MONOTONIC:
      ret = clock_gettime((clockid_t) clockid, &ts);
      result = ts.tv_sec * 1e9l + ts.tv_nsec;
      break;
    case CLOCK_REALTIME:
      ret = clock_gettime((clockid_t) clockid, &ts);
      result = ts.tv_sec * 1e9l + ts.tv_nsec;
      break;
    case CLOCK_PROCESS_CPUTIME_ID:
      ret = clock_gettime((clockid_t) clockid, &ts);
      result = ts.tv_sec * 1e9l + ts.tv_nsec;
      break;
    case CLOCK_THREAD_CPUTIME_ID:
      ret = clock_gettime((clockid_t) clockid, &ts);
      result = ts.tv_sec * 1e9l + ts.tv_nsec;
      break;
    default:
      Is_True(false, (NYI));
      ret = 0;
      result = 0;
  }

  *base_addr = result;
  m.Reg_i32().Set(0, 0); // errno.success
  Is_Trace(Tracing(TP_EXEC),
           (TFile, SYSCALL_PREFIX"clock_time_get(\"%d, %lld, %p\") = %d, sysret = %d, result=%lld\n",
             clockid, precision, base_addr, ret, ret, result));
}

/*
 * int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
 *                        void *(*start_routine) (void *), void *arg);
 */
void U_RUNTIME::VM_SYSCALL_pthread_create(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[4] = {I32, I32, I32, I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 4, TFile));
  char *thread = m.Get_base_addr(RUNTIME_PARM_REG(0));
  char *attr = m.Get_base_addr(RUNTIME_PARM_REG(1));
  char *start_routine = m.Get_base_addr(RUNTIME_PARM_REG(2));
  char *arg = m.Get_base_addr(RUNTIME_PARM_REG(3));
  INT32 routine_func_idx = *((INT32 *) start_routine);
  FUNC_INFO *func_info = Get_func_info(routine_func_idx);
  void *(*routine) (void *) = (void *(*) (void *)) func_info->Get_native_ptr();
  INT32 ret = pthread_create((pthread_t *) thread, (pthread_attr_t *) attr, routine, (void *) arg);
  m.Reg_i32().Set(0, ret);
  Is_Trace(Tracing(TP_EXEC),
           (TFile, SYSCALL_PREFIX"pthread_create(%p, %p, %d, %p), ret=%d\n",
            thread, attr, routine_func_idx, arg, ret));
}

/*
 * int pthread_join(pthread_t thread, void **retval);
 */
void U_RUNTIME::VM_SYSCALL_pthread_join(UWASM_MACHINE &m) {
  VALTYPE parm_ty_arr[2] = {I32, I32};
  Is_Trace_cmd(Tracing(TP_EXEC), Print_syscall_parm(m, parm_ty_arr, 2, TFile));
  UINT32 thread = m.Reg_i32().Get(RUNTIME_PARM_REG(0));
  void *retval;
  INT32 ret = pthread_join(thread, &retval);
  Is_Trace(Tracing(TP_EXEC),
          (TFile, SYSCALL_PREFIX"pthread_join(%d, %p), ret=%d\n",
          thread, retval, ret));
}

INT32 U_RUNTIME::U_malloc(UWASM_MACHINE &m, UINT64 size) {
  // returned offset is based on heap pointer, need to add heap offset
  uvm_ptr_t offset = UVM_heap_malloc(m.Heap_pointer(), size);
  return m.Heap_offset() + offset;
}

void U_RUNTIME::U_free(UWASM_MACHINE &m, INT32 mem) {
  UVM_heap_free((void *) m.Heap_pointer(), m.Heap_offset() + mem);
}

void U_RUNTIME::Init_name_call_ptr_cache() {
  _name_ptr_cache["zero"] = VM_SYSCALL_zero;
  _name_ptr_cache["one"] = VM_SYSCALL_one;
  _name_ptr_cache["trap"] = VM_SYSCALL_trap;
  _name_ptr_cache["putchar"] = VM_SYSCALL_putchar;
  _name_ptr_cache["malloc"] = VM_SYSCALL_malloc;
  _name_ptr_cache["free"] = VM_SYSCALL_free;
  _name_ptr_cache["proc_exit"] = VM_SYSCALL_proc_exit;
  _name_ptr_cache["uwasm_putlong"] = VM_SYSCALL_uwasm_putlong;
  _name_ptr_cache["uwasm_putchar"] = VM_SYSCALL_uwasm_putchar;
  _name_ptr_cache["uwasm_printfi"] = VM_SYSCALL_uwasm_printfi;
  _name_ptr_cache["uwasm_getint"] = VM_SYSCALL_uwasm_getint;
  _name_ptr_cache["uwasm_getchar"] = VM_SYSCALL_uwasm_getchar;
  _name_ptr_cache["uwasm_scanfv"] = VM_SYSCALL_uwasm_scanfv;
  _name_ptr_cache["uwasm_putfloat"] = VM_SYSCALL_uwasm_putfloat;
  _name_ptr_cache["uwasm_putdouble"] = VM_SYSCALL_uwasm_putdouble;
  _name_ptr_cache["uwasm_putstr"] = VM_SYSCALL_uwasm_putstr;
  _name_ptr_cache["uwasm_randii"] = VM_SYSCALL_uwasm_randii;
  _name_ptr_cache["uwasm_rand"] = VM_SYSCALL_uwasm_rand;
  _name_ptr_cache["uwasm_exit"] = VM_SYSCALL_proc_exit;
  _name_ptr_cache["uwasm_assert_true"] = VM_SYSCALL_uwasm_assert_true;
  _name_ptr_cache["uwasm_putint"] = VM_SYSCALL_uwasm_putint;
  _name_ptr_cache["clock_gettime"] = VM_SYSCALL_clock_gettime;
  _name_ptr_cache["clock_time_get"] = VM_SYSCALL_clock_time_get; // wasi definition
  _name_ptr_cache["pthread_create"] = VM_SYSCALL_pthread_create;
  _name_ptr_cache["pthread_join"] = VM_SYSCALL_pthread_join;
  _name_ptr_cache["fd_write"] = VM_SYSCALL_fd_write;
  _name_ptr_cache["args_get"] = VM_SYSCALL_args_get;
  _name_ptr_cache["args_sizes_get"] = VM_SYSCALL_args_sizes_get;
  _name_ptr_cache["env.uwasm_putlong"] = VM_SYSCALL_uwasm_putlong;
  _name_ptr_cache["env.uwasm_putfloat"] = VM_SYSCALL_uwasm_putfloat;
  _name_ptr_cache["env.uwasm_putdouble"] = VM_SYSCALL_uwasm_putdouble;
  _name_ptr_cache["env.uwasm_putstr"] = VM_SYSCALL_uwasm_putstr;
  _name_ptr_cache["wasi_snapshot_preview1.clock_gettime"] = VM_SYSCALL_clock_gettime;
  _name_ptr_cache["wasi_snapshot_preview1.pthread_create"] = VM_SYSCALL_pthread_create;
  _name_ptr_cache["wasi_snapshot_preview1.pthread_join"] = VM_SYSCALL_pthread_join;
  _name_ptr_cache["wasi_snapshot_preview1.fd_write"] = VM_SYSCALL_fd_write;
  _name_ptr_cache["wasi_snapshot_preview1.proc_exit"] = VM_SYSCALL_proc_exit;
}

static const char *ffi_native_call[] = {
  "putchar"
};

void U_RUNTIME::Init_cache(U_MODULE &u_module) {
  Init_name_call_ptr_cache();
  if (!u_module.Contains_section(S_IMP_NAT)) {
    Is_Trace(Tracing(TP_BIN_READ), (TFile, "no IMPORT section found.\n"));
    return;
  }
  Reserve_func_info(u_module.Get_shdr_by_sec_id(SEC_FUNC).Get_elem_num());
  UBH_SHDR &imp_nat_shdr = u_module.Get_shdr_by_sec_id(S_IMP_NAT);
  for (UINT32 i = 0; i < imp_nat_shdr.Get_elem_num(); i++) {
    UBH_IMP_NAT &imp_nat_entry = u_module.Get_imp_nat(i);
    UINT32 func_idx = imp_nat_entry.Get_func_idx();
    UBH_FUNC &func = u_module.Get_func(func_idx);
    UINT32 ntype_off = imp_nat_entry.Get_ntype_off();
    FUNC_INFO *func_info = new FUNC_INFO(func_idx);
    char *func_name = u_module.Get_module_name(func.Get_name_idx());
    if (_name_ptr_cache.find(func_name) != _name_ptr_cache.end()) {
        // cache K-V: func idx, sys call func pointer, syscall will overwrite ffi call
        func_info->Init_syscall(_name_ptr_cache[func_name]);
        Set_func_info(func_idx, func_info);
    } else if (imp_nat_entry.Get_nat_off() != 0xffffff) {
      UBH_NAT &nat_entry = u_module.Get_nat_entry(imp_nat_entry.Get_nat_idx());
      char* func_ptr = (nat_entry.Get_buf() + imp_nat_entry.Get_nat_off());
      ffi_cif *cif = Create_cif(u_module, func.Get_type_idx(), ntype_off);
      func_info->Init_native(ntype_off, (NAT_CALL_PTR)func_ptr, cif);
      Set_func_info(func_idx, func_info);
      //printf("fun[%d] %s addr:0x%x\n", func_idx, func_name, func_ptr);
    } else {
      BOOL found = false;
      for (UINT32 j = 0; j < sizeof(ffi_native_call) / sizeof(char *); j++) {
        if (strcmp(func_name, ffi_native_call[j]) == 0) {
          NAT_CALL_PTR ptr = (NAT_CALL_PTR) dlsym(NULL, ffi_native_call[j]);
          Is_True(ptr != NULL, ("dlsym failed, function nema: %s.", ffi_native_call[j]));
          ffi_cif *cif = Create_cif(u_module, func.Get_type_idx(), ntype_off);
          func_info->Init_native(ntype_off, ptr, cif);
          Set_func_info(func_idx, func_info);
          found = true;
        }
      }
      if (found == false) {
        fprintf(stderr, "Warn: unknown extern func[%d]:%s\n", func_idx, func_name);
#ifndef SKIP_EXTERNAL_FUNCTION
        func_info->Init_syscall(_name_ptr_cache["trap"]);
        Set_func_info(func_idx, func_info);
#endif
      }
    }
  }
}

static ffi_type* NTYPE_TO_FFITYPE[] = {
  &ffi_type_void,
  &ffi_type_uint8,
  &ffi_type_sint8,
  &ffi_type_uint16,
  &ffi_type_sint16,
  &ffi_type_uint32,
  &ffi_type_sint32,
  &ffi_type_uint64,
  &ffi_type_sint64,
  &ffi_type_float,
  &ffi_type_double,
  &ffi_type_pointer,
};

ffi_cif *U_RUNTIME::Create_cif(U_MODULE &u_module, UINT32 type_idx, UINT32 ntype_off) {
  UBH_TYPE &ubh_type = u_module.Get_type(type_idx);
  UINT8 *ntype_arr = (UINT8 *) u_module.Get_non_null_str(ntype_off);
  UINT32 ret_num = u_module.Get_ret_num(type_idx);
  UINT32 param_num = u_module.Get_param_num(type_idx);
  ffi_type *ret_type = NULL;
  if (ret_num == 1) {
    UINT8 ret_ntype = ntype_arr[0];
    Is_True(ret_ntype < NTYPE_NUM, ("return ntype exceeds the NTYPE_NUM, return ntype: %d.", ret_ntype));
    ret_type = NTYPE_TO_FFITYPE[ret_ntype];
    ntype_arr++;
  } else if (ret_num == 0) {
    ret_type = &ffi_type_void;
  } else {
    Is_True(false, ("Unsupported return number: %d.", ret_num));
  }
  ffi_type **param_arr = NULL;
  if (param_num > 0) {
    param_arr = (ffi_type **) malloc(param_num * sizeof(ffi_type *));
  }
  for (UINT32 i = 0; i < param_num; i++) {
    UINT8 param_ntype = ntype_arr[i];
    Is_True(param_ntype < NTYPE_NUM, ("parameter ntype exceeds the NTYPE_NUM, param ntype: %d.", param_ntype));
    param_arr[i] = NTYPE_TO_FFITYPE[param_ntype];
  }
  ffi_cif *cif = (ffi_cif *) malloc(sizeof(ffi_cif));
  ffi_status status = ffi_prep_cif(cif, FFI_DEFAULT_ABI, param_num, ret_type, param_arr);
  Is_True(status == FFI_OK, ("FFI status is not ok, status: %d.", status));
  return cif;
}

void U_RUNTIME::Call_sys_call(UWASM_MACHINE &m, UINT32 func_idx) {
  U_MODULE &u_module = m.Get_u_module();
  UBH_FUNC &func = u_module.Get_func(func_idx);
  FUNC_INFO *func_info = Get_func_info(func_idx);
  const char *func_name = u_module.Get_module_name(func.Get_name_idx());
#ifdef SKIP_EXTERNAL_FUNCTION
  if (func_info == nullptr) {
    Is_Trace(Tracing(TP_EXEC), (TFile, "Skip syscall for debugging, func idx: %d, func name: %s.\n", func_idx, func_name));
    return;
  }
#endif
  if(func_info) {
    if(func_info->Is_syscall()) {
      Is_Trace(Tracing(TP_EXEC), (TFile, "Manually syscall, func idx: %d, func name: %s.\n", func_idx, func_name));
      SYS_CALL_PTR sys_call_ptr = func_info->Get_sys_ptr();
      sys_call_ptr(m);
    } else if(func_info->Is_native()) {
      Is_Trace(Tracing(TP_EXEC), (TFile, "FFI call, func idx: %d, func name: %s.\n", func_idx, func_name));
      Ffi_call(m, func_info);
    } else {
      Is_True(0, ("Invalid func_info type: func idx: %d, func name: %s.", func_idx, func_name));
    }
  } else {
    fprintf(stderr, "Error: can't find native call/syscall, func idx: %d, func name: %s\n", func_idx, func_name);
    exit(-1);
  }
}

void U_RUNTIME::Ffi_call(UWASM_MACHINE &m, FUNC_INFO* func_info) {
  U_MODULE &u_module = m.Get_u_module();
  UINT32 func_idx = func_info->Get_func_idx();
  UBH_FUNC &func = u_module.Get_func(func_idx);
  UINT32 type_idx = func.Get_type_idx();
  UINT32 ntype_off = func_info->Get_ntype_ofs();
  ffi_cif *cif = func_info->Get_cif();
  NAT_CALL_PTR native_ptr = func_info->Get_native_ptr();
  Is_True(cif, ("Ffi call: cif is null for fun %d: %s",
               func_idx, u_module.Get_module_name(func.Get_name_idx())));
  Is_True(native_ptr, ("Ffi call, native function pointer is null for fun %d: %s",
                       func_idx, u_module.Get_module_name(func.Get_name_idx())));

  UINT8 *ntype_arr = (UINT8 *) u_module.Get_non_null_str(ntype_off);
  UINT32 ret_num = u_module.Get_ret_num(type_idx);
  UINT32 param_num = u_module.Get_param_num(type_idx);
  void *ret = NULL;
  UINT32 param_idx[4] = {0, 0, 0, 0};
  UINT8 ret_ntype = ntype_arr[0];
  if (ret_num > 0) {
    ntype_arr++;
    UINT8 ret_type = u_module.Get_ret_type(type_idx, 0);
    if (ret_ntype == NTYPE_PTR) {
      Is_True(ret_type == I32, ("Native type is pointer, but ubh type is not I32, ubh type: %d.", ret_type));
      ret = &POINT_RET;
    } else {
      if (ret_type == I32) {
        ret = m.Reg_i32().Get_addr(param_idx[ret_type]);
      } else if (ret_type == I64) {
        ret = m.Reg_i64().Get_addr(param_idx[ret_type]);
      } else if (ret_type == F32) {
        ret = m.Reg_f32().Get_addr(param_idx[ret_type]);
      } else if (ret_type == F64) {
        ret = m.Reg_f64().Get_addr(param_idx[ret_type]);
      }
    }
    param_idx[ret_type]++;
  }
  if (param_num > 0) {
    for (UINT32 i = 0; i < param_num; i++) {
      UINT8 param_ntype = ntype_arr[i];
      UINT8 param_type = u_module.Get_param_type(type_idx, i);
      // pointer type, should convert offset to base addr, then pass to ffi call
      // ATTENTION: pointer type parameter, should take parameter address, saved to POINT_PARAM_ARR
      //            then take the element's address, saved to ACTUAL_ARR, this is the requirement from ffi
      if (param_ntype == NTYPE_PTR) {
        Is_True(param_type == I32, ("Native type is pointer, but ubh type is not I32, ubh type: %d.", param_type));
        UINT32 reg_idx = param_idx[param_type]++;
        INT32 off = m.Reg_i32().Get(reg_idx);
        if (off != 0) {
          POINT_PARAM_ARR[i]  = m.Get_base_addr(reg_idx);
        } else {
          // offset can't be zero, if offset is zero, means this is a NULL pointer
          POINT_PARAM_ARR[i] = NULL;
        }
        ACTUAL_ARR[i] = POINT_PARAM_ARR + i;
      } else {
        if (param_type == I32) {
          ACTUAL_ARR[i] = m.Reg_i32().Get_addr(param_idx[param_type]++);
        } else if (param_type == I64) {
          ACTUAL_ARR[i] = m.Reg_i64().Get_addr(param_idx[param_type]++);
        } else if (param_type == F32) {
          ACTUAL_ARR[i] = m.Reg_f32().Get_addr(param_idx[param_type]++);
        } else if (param_type == F64) {
          ACTUAL_ARR[i] = m.Reg_f64().Get_addr(param_idx[param_type]++);
        } else {
          Is_True(false, ("Unsupported type."));
        }
      }
    }
  }
  ffi_call(cif, native_ptr, ret, ACTUAL_ARR);
  if (ret_num > 0 && ret_ntype == NTYPE_PTR) {
    char *ret_addr = (char *)(*(INTPTR *)ret);
#ifdef DIRECT_ACC_MEM
    m.Reg_i32().Set(0, (INTPTR)ret_addr);
#else
    INT32 off = 0;
    if (ret_addr - m.Cur_env().Get_sp() >= 0 ) {
      off = ret_addr - m.Stack_pointer();
    } else {
      off = ret_addr - m.Mem_pointer();
    }
    m.Reg_i32().Set(0, off);
#endif
  }
}
