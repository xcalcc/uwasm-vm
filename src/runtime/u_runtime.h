/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef UWASM_VM_U_RUNTIME_H
#define UWASM_VM_U_RUNTIME_H

#include <map>
#include <string.h>
#include "u_types.h"
#include "ffi.h"
#include "ubh/u_module.h"

#ifdef UWASM_VM_RELEASE
#ifdef SKIP_EXTERNAL_FUNCTION
#undef SKIP_EXTERNAL_FUNCTION
#endif
#else
#define SKIP_EXTERNAL_FUNCTION true
#endif

using namespace std;

struct c_str_cmp
{
  bool operator()(char const *a, char const *b) const {
    return strcmp(a, b) < 0;
  }
};
class UWASM_MACHINE;

typedef void (*SYS_CALL_PTR) (UWASM_MACHINE &);
typedef void (*NAT_CALL_PTR) ();
typedef map<const char *, SYS_CALL_PTR, c_str_cmp> NAME_PTR_MAP;

enum FUNC_TYPE {
  FUNC_IS_UWASM,
  FUNC_IS_NATIVE,
  FUNC_IS_SYSCALL
};
class FUNC_INFO
{
private:
  UINT32                             _func_idx;    // function index
  UINT32                             _ntype_ofs;   // native function type offset
  FUNC_TYPE                          _type;        // func is uwasm/native/syscall
  union {
    SYS_CALL_PTR                     _syscall_ptr; // syscall function address
    NAT_CALL_PTR                     _native_ptr;  // native function address
  } _func_ptr;
  ffi_cif*                           _cif;         // call interface

public:
  FUNC_INFO(UINT32 idx) : _func_idx(idx), _type(FUNC_IS_UWASM),
                          _ntype_ofs(0), _cif(NULL)
                          { _func_ptr._native_ptr = NULL; }

  void Init_native(UINT32 off, NAT_CALL_PTR ptr, ffi_cif *cif)
  {
    _type = FUNC_IS_NATIVE;
    _func_ptr._native_ptr = ptr;
    _cif = cif;
    _ntype_ofs = off;
  }

  void Init_syscall(SYS_CALL_PTR ptr)
  {
    _type = FUNC_IS_SYSCALL;
    _func_ptr._syscall_ptr = ptr;
  }

  UINT32        Get_func_idx(void) const   { return _func_idx; }
  UINT32        Get_ntype_ofs(void) const  { return _ntype_ofs; }
  ffi_cif      *Get_cif(void) const        { return _cif; }
  SYS_CALL_PTR  Get_sys_ptr(void) const    { Is_True(Is_syscall(), ("sys call function is native type"));
                                             return _func_ptr._syscall_ptr; }
  NAT_CALL_PTR  Get_native_ptr(void) const { Is_True(Is_native(), ("native function is sys call type"));
                                             return _func_ptr._native_ptr; }
  bool          Is_native(void) const      { return _type == FUNC_IS_NATIVE;  }
  bool          Is_syscall(void) const     { return _type == FUNC_IS_SYSCALL; }
};

class U_RUNTIME {
private:
  static NAME_PTR_MAP                       _name_ptr_cache;
  static vector<FUNC_INFO *>                _func_infos;

private:
  static void           Init_name_call_ptr_cache();
  static ffi_cif       *Create_cif(U_MODULE &module, UINT32 type_idx, UINT32 ntype_off);
  static INT32          U_malloc(UWASM_MACHINE &m, UINT64 size);
  static void           U_free(UWASM_MACHINE &m, INT32 mem);
  static void           Ffi_call(UWASM_MACHINE &m, FUNC_INFO* func_info);
  static void           Reserve_func_info(UINT32 n)                     { _func_infos.resize(n);        }
  static FUNC_INFO     *Get_func_info(UINT32 func_idx)                  {
#ifdef SKIP_EXTERNAL_FUNCTION
    if (func_idx >= _func_infos.size()) {
      return nullptr;
    }
#endif
    Is_True(_func_infos.size() > func_idx,
            ("Invalid func_idx = %u for external call, only %lu available",
             func_idx, _func_infos.size()));
    return _func_infos[func_idx];
  }
  static void           Set_func_info(UINT32 func_idx, FUNC_INFO *info) { _func_infos[func_idx] = info; }

public:
  static void           Init_cache(U_MODULE &module);
  static void           Call_sys_call(UWASM_MACHINE &m, UINT32 func_idx);
  static void           VM_SYSCALL_putchar(UWASM_MACHINE &m);
  static void           VM_SYSCALL_malloc(UWASM_MACHINE &m);
  static void           VM_SYSCALL_free(UWASM_MACHINE &m);
  static void           VM_SYSCALL_uwasm_putlong(UWASM_MACHINE &m);
  static void           VM_SYSCALL_uwasm_putint(UWASM_MACHINE &m);
  static void           VM_SYSCALL_uwasm_putfloat(UWASM_MACHINE &m);
  static void           VM_SYSCALL_uwasm_putstr(UWASM_MACHINE &m);
  static void           VM_SYSCALL_uwasm_putdouble(UWASM_MACHINE &m);
  static void           VM_SYSCALL_uwasm_randii(UWASM_MACHINE &m);
  static void           VM_SYSCALL_uwasm_rand(UWASM_MACHINE &m);
  static void           VM_SYSCALL_uwasm_printfi(UWASM_MACHINE &m);
  static void           VM_SYSCALL_uwasm_scanfv(UWASM_MACHINE &m);
  static void           VM_SYSCALL_uwasm_putchar(UWASM_MACHINE &m);
  static void           VM_SYSCALL_uwasm_getchar(UWASM_MACHINE &m);
  static void           VM_SYSCALL_uwasm_getint(UWASM_MACHINE &m);
  static void           VM_SYSCALL_args_sizes_get(UWASM_MACHINE &m);
  static void           VM_SYSCALL_args_get(UWASM_MACHINE &m);
  static void           VM_SYSCALL_uwasm_assert_true(UWASM_MACHINE &m);
  static void           VM_SYSCALL_proc_exit(UWASM_MACHINE &m);
  static void           VM_SYSCALL_one(UWASM_MACHINE &m);
  static void           VM_SYSCALL_zero(UWASM_MACHINE &m);
  static void           VM_SYSCALL_trap(UWASM_MACHINE &m);
  static void           VM_SYSCALL_clock_gettime(UWASM_MACHINE &m);
  static void           VM_SYSCALL_clock_time_get(UWASM_MACHINE &m);
  static void           VM_SYSCALL_pthread_create(UWASM_MACHINE &m);
  static void           VM_SYSCALL_pthread_join(UWASM_MACHINE &m);
  static void           VM_SYSCALL_fd_write(UWASM_MACHINE &m);
  static void           Print_syscall_parm(UWASM_MACHINE &m, VALTYPE parm_ty_arr[], UINT32 parm_len, FILE *file);
  static void           Print_syscall_parm(UWASM_MACHINE &m, VALTYPE ret_ty, VALTYPE parm_ty_arr[], UINT32 parm_len, FILE *file);
  static void           Print_syscall_ret(UWASM_MACHINE &m, VALTYPE ret_ty, FILE *file);
};

#endif //UWASM_VM_U_RUNTIME_H
