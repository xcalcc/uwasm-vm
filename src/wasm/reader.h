/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef WASM2UWASM_READER_H
#define WASM2UWASM_READER_H

#include "utilities/u_utils.h"
#include "ubh/u_opcode.h"
#include "trace/u_trace.h"
#include "winst.h"
#include "ir.h"
#include <map>

typedef UINT32 FUNC_IDX;

typedef enum {
  FILE_WASM  = 1,
  FILE_UWASM = 2,
} WASM_FILE_KIND;

class FILE_CONTEXT {
private:
  WASM_FILE_KIND    _kind;             // wasm or uwasm
  BOOL              _emit_binary;      // whether to emit binary dump
  BOOL              _full_file;        // whether the file contains all sections
  wabt::Module     *_module;           // wabt's module
  FUNC_IDX          _func_idx;         // current function index
  UINT32            _last_anon_func_idx;
  UINT32            _last_anon_glbl_idx;
  UINT32            _import_globals;
  BOOL              _stack_check;     // whether enabling the end of function stack check
  std::map<UINT32, std::string> _func_name_cache; // func name cache
  std::map<UINT32, std::string> _glbl_name_cache; // global name cache
  std::map<UINT32, UINT32>      _glbl_id_map;     // global id map
public:
  WASM_FILE_KIND     Kind()                const   { return _kind;                                       }
  BOOL               Is_emit_binary()      const   { return _emit_binary;                                }
  BOOL               Is_full_file()        const   { return _full_file;                                  }
  BOOL               Is_stack_check()      const   { return _stack_check;                                }
  wabt::Func        *Get_func()            const   { return Get_module()->GetFunc(wabt::Var(_func_idx)); }
  wabt::Func        *Next_func()                   { _func_idx++; return Get_func();                     }
  FUNC_IDX           Get_func_idx()        const   { return _func_idx;                                   }
  UINT32            &Get_last_func_idx()           { return _last_anon_func_idx;                         }
  UINT32            &Get_last_glbl_idx()           { return _last_anon_glbl_idx;                         }
  UINT32             Get_import_global_cnt()       {  return _import_globals;                            }
  wabt::Module      *Get_module()          const   {
    Is_True(_module != nullptr, ("module is null now."));
    return _module;
  }

  void      Set_kind (WASM_FILE_KIND kind)         {  _kind = kind;                                      }
  void      Set_emit_binary (BOOL opt)             {  _emit_binary = opt;                                }
  void      Set_full_file (BOOL opt)               {  _full_file = opt;                                  }
  void      Set_stack_check (BOOL opt)             {  _stack_check = opt;                                }
  void      Set_module(wabt::Module &module)       {  _module = &module;                                 }
  void      Set_func_ctx(FUNC_IDX func_ctx)        {  _func_idx = func_ctx;                              }
  void      Inc_import_global_cnt()                {  _import_globals++;                                 }
  FILE_CONTEXT() {
    _emit_binary = true;
    _full_file   = false;
    _stack_check = false;
    _kind        = FILE_UWASM;
    _module      = nullptr;
    _last_anon_func_idx = 0;
    _last_anon_glbl_idx = 0;
    _import_globals = 0;
  }
  std::string Get_anon_func_name();
  std::string Get_anon_global_name();
  void Set_func_name_cache(UINT32 id, std::string name) {
    Is_True(_func_name_cache.find(id) == _func_name_cache.end(), ("Already added name cache for func id = %d.", id));
    _func_name_cache.insert(std::make_pair(id, name));
  }
  const char *Get_func_name_cache(UINT32 id) {
    if (_func_name_cache.find(id) == _func_name_cache.end()) {
      return nullptr;
    }
    return _func_name_cache.at(id).c_str();
  }
  void Set_global_name_cache(UINT32 id, std::string name) {
    Is_True(_glbl_name_cache.find(id) == _glbl_name_cache.end(), ("Already added name cache for global id = %d.", id));
    _glbl_name_cache.insert(std::make_pair(id, name));
  }
  const char *Get_global_name_cache(UINT32 id) {
    if (_glbl_name_cache.find(id) == _glbl_name_cache.end()) {
      return nullptr;
    }
    return _glbl_name_cache.at(id).c_str();
  }
  void Set_global_id_map(UINT32 wasm_id, UINT32 uwasm_id) {
    _glbl_id_map.insert(std::make_pair(wasm_id, uwasm_id));
  }
  UINT32 Get_global_uwasm_id(UINT32 wasm_id) {
    if (_glbl_id_map.find(wasm_id) == _glbl_id_map.end()) {
      return 0;
    }
    return _glbl_id_map.at(wasm_id);
  }
};

// Utility function for reading a constant with uint8 format. (one-byte)
//
UINT8 Read_uint8(CONST_BUFFER pc);

// Utility function for reading an unsigned constant with Leb128 format.
//
UINT64 Read_leb128(CONST_BUFFER pc);

// Utility function for reading a signed constant with Leb128 format.
//
INT64 Read_signed_leb128(CONST_BUFFER pc);

// Utility function for reading a opr's size with type = I32/I64. (Leb128 format)
//
UINT8  Read_opr_size_leb128(CONST_BUFFER pc);

// Utility function for reading a constant.
//
UINT64 Read_const_value(CONST_BUFFER pc, VALTYPE valty);

template <typename T>
T Read_uint(CONST_BUFFER pc, UINT32 _sz=0) {
  UINT32 _res_sz = sizeof(T) * 8;
  if (_sz == 0) _sz = _res_sz;
  T res = *(T *)pc;
  res &= (~((T)0x0) >> (_res_sz - _sz));
  return res;
}

WINSVEC Read_insts(CONST_BUFFER inst_buf, UINT32 buf_len, FILE_CONTEXT &ctx);
INT32 Read_all_sections(CONST_BUFFER module_buf, UINT32 length, FILE_CONTEXT &ctx);
INT32 Read_magic_number(CONST_BUFFER buffer, UINT32 &pc, UINT32 length);
UINT32 Read_available(CONST_BUFFER buffer, UINT32 &pc, UINT32 length);
void Save_wasm_pc(CONST_BUFFER pc, WASM_INST &inst);


#endif //WASM2UWASM_READER_H
