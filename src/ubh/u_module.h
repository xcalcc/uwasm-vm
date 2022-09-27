/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by xc5 on 2020/9/14.
//

#ifndef UWASM_VM_U_MODULE_H
#define UWASM_VM_U_MODULE_H

#include <vector>
#include <map>
#include "ubh/ubh.h"
#include "uvm/uvm.h"
#include "u_types.h"
#include "trace/u_trace.h"
#include <unordered_map>
#include <unordered_set>

using namespace std;
extern TY_AM UWINST_TY_AM[];

/**
 * @description: expand address mode to avoid expanding in execution
 * at most three fields in am are expanded
 * for each am type, register index fields are in order, e.g. AM_64_R_R_R:
 * | opcode_l | valtype | rd | rs | rt |
 * |   opcode_l(long)   | e1 | e2 | e3 |
 * imm and ofs fields are always placed in e3, e.g. AM_32_R_OFS:
 * | e1: rd | e2: null | e3: ofs |
 * */
struct AM_EXPAND {
private:
  UINT32     _opcode;
  UINT32     _expand1;
  UINT32     _expand2;
  UINT32     _expand3;

public:
  void Print(FILE *file=stderr) const;

  UINT32 Get_opcode_l()               const { return _opcode;  }
  UINT32 Get_rd()                   const { return _expand1; }
  UINT32 Get_rs()                   const { return _expand2; }
  UINT32 Get_ofs()                  const { return _expand3; }
  INT32  Get_imm()                  const { return (INT32) _expand3; }
  UINT32 Get_rt()                   const { return _expand3; }

  void Set_opcode(UINT32 opcode)          { _opcode = opcode; }
  void Set_rd(UINT32 rd)                  { _expand1 = rd;    }
  void Set_rs(UINT32 rd)                  { _expand2 = rd;    }
  void Set_rt(UINT32 rt)                  { _expand3 = rt;    }
  void Set_ofs(UINT32 ofs)                { _expand3 = ofs;   }
  void Set_imm(UINT32 imm)                { _expand3 = imm;   }
};

/*
 * CODE SEC:
 *    code info
 *    instruction table
 *    label table
 */
class U_MODULE {
  friend class UWASM_MACHINE;
private:
  UBH_MHDR          *_mhdr;
  UBH_SHDR          *_shdr_tab;
  UBH_TYPE          *_type_sec;
  UBH_IMPORT        *_import_sec;
  UBH_IMP_NAT       *_imp_nat_sec;
  UBH_EXPORT        *_export_sec;
  UBH_FUNC          *_func_sec;
  UINT32            *_reloc_sec;
  UBH_EXP_GLBL      *_exp_glbl_sec;
  char              *_data_sec;
  UBH_CODE         **_code_sec;
  UBH_GLBL          *_glbl_sec;
  char              *_non_null_str_sec;
  char              *_module_name_sec;
  char              *_glbl_name_sec;
  UBH_NAT          **_nat_sec;
  map<UINT8, UINT8>  _sec_sec_header_map;
  char              *_real_data_sec;       // need to copy data section, original data section maybe don't have write permission
  char              *_buffer;
  char               _buffer_len;

  // the following member are cache
  AM_EXPAND         *_expanded_table;      // need deallocation, expanded instructions
  AM_EXPAND        **_expanded_func_base;  // need deallocation, start inst of each function in expanded table
  UINT32            *_param_num_tab;       // need deallocation, decoded param num, param_num_tab[type_idx*4 + valty] is the param num of valty X
  UINT32            *_ret_num_tab;         // need deallocation, decoded ret num
  vector<UINT8>     *_param_tab;           // need deallocation, cached param in type
  vector<UINT8>     *_ret_tab;             // need deallocation, cached ret in type
  AM_EXPAND       ***_lab_tab;             // need deallocation, decoded label table
  AM_EXPAND         *_end_pc;              // break inst pc
  UINT64            *_reg_num_tab;         // need deallocation, cache reg num table.
  UINT64            *_fp_shs_tab;          // need deallocation, cache fp shift size table.

  // debug info
  UINT32            *_expanded_pc_map;     // need deallocation, map expanded pc to origin pc
  UINT32            *_origin_pc_map;       // need deallocation, map origin pc to expanded pc
  bool               _keep_off;            // part of section field saved as offset, can be convert to pointer, keep the state

  // PC-Expand Mapping.
  // How to use
  unordered_map<UINT64,      AM_EXPAND *> _pc_to_exp_map;
  unordered_map<AM_EXPAND *, UINT64>      _exp_to_pc_map;
private:
  UINT32        Fix_up_reloc(char *mem_p, UINT32 mem_size);
  void          Decode_type_and_cache();
  void          Expand_code();
  void          Convert_off_to_ptr();
  void          Convert_ptr_to_off();

public:
  U_MODULE();
  ~U_MODULE();
  UBH_MHDR     &Mhdr()                                      const { return *_mhdr; }
  UBH_SHDR     *Shdr_tab()                                  const { return _shdr_tab; }
  UBH_SHDR     &Get_shdr(INT32 idx)                         const { return _shdr_tab[idx]; }
  UBH_TYPE     *Type_sec()                                  const { return _type_sec; }
  UBH_TYPE     &Get_type(INT32 idx)                         const { return _type_sec[idx]; }
  UBH_IMPORT   *Import_sec()                                const { return _import_sec; }
  UBH_IMPORT   &Get_import(UINT32 idx)                      const { return _import_sec[idx]; }
  UBH_IMP_NAT  *Imp_nat_sec()                               const { return _imp_nat_sec; }
  UBH_IMP_NAT  &Get_imp_nat(UINT32 idx)                     const { return _imp_nat_sec[idx]; }
  UBH_EXPORT   *Export_sec()                                const { return _export_sec; }
  UBH_EXPORT   &Get_export(UINT32 idx)                      const { return _export_sec[idx]; }
  UBH_FUNC     *Func_sec()                                  const { return _func_sec; }
  UBH_FUNC     &Get_func(INT32 idx)                         const { return _func_sec[idx]; }
  UBH_GLBL     *Glbl_sec()                                  const { return _glbl_sec; }
  UBH_GLBL     &Get_glbl(INT32 idx)                         const { return _glbl_sec[idx]; }
  UINT32       *Reloc_sec()                                 const { return _reloc_sec; }
  UINT32       *Get_reloc(INT32 idx)                        const { return _reloc_sec + idx; }
  UBH_EXP_GLBL *Get_exp_glbl(INT32 idx)                     const { return _exp_glbl_sec; }
  char         *Data_sec()                                  const { return _data_sec; }
  char         *Get_data(INT32 idx)                         const { return _data_sec + idx; }
  UBH_CODE    **Code_sec()                                  const { return _code_sec; }
  UBH_CODE     &Get_code(INT32 idx)                         const { return *(_code_sec[idx]); }
  char         *Non_null_str_sec()                          const { return _non_null_str_sec; }
  char         *Get_non_null_str(INT32 idx)                 const { return _non_null_str_sec + idx; }
  char         *Module_name_sec()                           const { return _module_name_sec; }
  char         *Get_module_name(INT32 idx)                  const { return _module_name_sec + idx; }
  char         *Glbl_name_sec()                             const { return _glbl_name_sec; }
  char         *Get_glbl_name(INT32 idx)                    const { return _glbl_name_sec + idx; }
  AM_EXPAND   **Get_label_tab(UINT32 func_idx)              const { return _lab_tab[func_idx]; }
  AM_EXPAND    *Get_end_pc()                                const { return _end_pc; }
  UINT64        Get_reg_num(UINT32 func_idx)                const { return _reg_num_tab[func_idx]; }
  UINT64        Get_fp_shs_num(UINT32 func_idx)             const { return _fp_shs_tab[func_idx]; }
  UINT8         Get_param_type(UINT32 type_idx, UINT8 idx)  const { return _param_tab[type_idx].at(idx); }
  UINT16        Get_param_num(UINT32 type_idx)              const { return _param_tab[type_idx].size(); }
  UINT8         Get_ret_type(UINT32 type_idx, UINT8 idx)    const { return _ret_tab[type_idx].at(idx); }
  UINT8         Get_ret_num(UINT32 type_idx)                const { return _ret_tab[type_idx].size(); }
  AM_EXPAND    *Get_expanded_inst(UINT32 i)                 const { return _expanded_table + i; }
  AM_EXPAND    *Get_expanded_func_base(UINT32 i)            const { return _expanded_func_base[i]; }
  UINT32        Get_pc_idx(AM_EXPAND *inst)                       { return (inst - _expanded_table) / sizeof(AM_EXPAND*); }
  UINT8         Get_sec_id_from_header(UBH_SHDR &sec_header) const;
  UBH_NAT      &Get_nat_entry(UINT32 idx)                   const { return *_nat_sec[idx]; }
  UBH_NAT     **Nat_sec()                                   const { return _nat_sec; }
  bool          Keep_off()                                  const { return _keep_off; }
  UINT32       *Expanded_pc_map()                           const { return _expanded_pc_map; }
  UINT32       *Origin_pc_map()                             const { return _origin_pc_map; }
  void          Set_header(UBH_MHDR *header)                      { _mhdr = header; }
  void          Set_shdr_tab(UBH_SHDR *shdr_tab);
  void          Set_type_sec(UBH_TYPE *type_sec)                  { _type_sec = type_sec; }
  void          Set_import_sec(UBH_IMPORT *import_sec)            { _import_sec = import_sec; }
  void          Set_imp_nat_sec(UBH_IMP_NAT *imp_nat_sec)         { _imp_nat_sec = imp_nat_sec; }
  void          Set_func_sec(UBH_FUNC *func_sec)                  { _func_sec = func_sec; }
  void          Set_reloc_sec(UINT32 *reloc_sec)                  { _reloc_sec = reloc_sec; }
  void          Set_export_sec(UBH_EXPORT *export_sec)            { _export_sec = export_sec; }
  void          Set_exp_glbl_sec(UBH_EXP_GLBL *exp_glbl_sec)      { _exp_glbl_sec = exp_glbl_sec; }
  void          Set_data_sec(char *data_sec)                      { _data_sec = data_sec; }
  void          Set_glbl_sec(UBH_GLBL *glbl_sec)                  { _glbl_sec = glbl_sec; }
  void          Set_non_null_str_sec(char *non_null_str_sec)      { _non_null_str_sec = non_null_str_sec; }
  void          Set_module_nmae_sec(char *module_name_sec)        { _module_name_sec = module_name_sec; }
  void          Set_glbl_name_sec(char *glbl_name_sec)            { _glbl_name_sec = glbl_name_sec; }
  void          Set_nat_sec(UBH_NAT *nat_sec);
  BOOL          Contains_section(UINT8 sec_id)              const { return _sec_sec_header_map.find(sec_id) !=
                                                                      _sec_sec_header_map.end(); }
  void          Set_code_sec(char *code_sec);
  void          Set_keep_off(bool keep_off)                       { _keep_off = keep_off; }
  UINT32        Prepare_before_run(char *mem_p, UINT32 mem_size);
  void          Mprotect_bin_buff();
  void          Print_header(FILE *file) const;                 // print module header, header just dump original data
  void          Print_sec_header(FILE *file) const;
  void          Print_one_type(FILE *file, UINT32 idx) const;
  void          Print_type_cache(FILE *file) const;
  void          Print_import_sec(FILE *file) const;
  void          Print_imp_nat_sec(FILE *file) const;
  void          Print_export_sec(FILE *file) const;
  void          Print_func_header(UINT32 func_idx, FILE *file) const;
  void          Print_func_sec(FILE *file);
  void          Print_data_sec(FILE *file) const;
  void          Print_glbl_sec(FILE *file) const;
  void          Print_non_null_str_tab(FILE *file) const;
  void          Print_nat_sec(FILE *file) const;
  void          Print(FILE *file=TFile);

  UINT32 Get_shdr_idx(UINT8 sec_id) const {
    Is_True(_sec_sec_header_map.find(sec_id) != _sec_sec_header_map.end(),
            ("Can't find sec id in cache, sec id: %d.", sec_id));
    return _sec_sec_header_map.find(sec_id)->second;
  }
  UBH_SHDR &Get_shdr_by_sec_id(UINT8 sec_id) const {
    UINT32 shdr_idx = Get_shdr_idx(sec_id);
    return Get_shdr(shdr_idx);
  }
  template<VALTYPE valty>
  UINT32    Get_proto_param_num(UINT32 type_idx) const {
    Is_True(_param_num_tab != NULL, ("table uninitialized"));
    Is_True(type_idx < Get_shdr_by_sec_id(S_TYPE).Get_elem_num(), ("index out of bound: %d", type_idx));
    return _param_num_tab[type_idx * VALTY_COUNT + valty];
  }
  template<VALTYPE valty>
  UINT32    Get_proto_ret_num(UINT32 type_idx) const {
    Is_True(_ret_num_tab != NULL, ("table uninitialized"));
    Is_True(type_idx < Get_shdr_by_sec_id(S_TYPE).Get_elem_num(), ("index out of bound: %d", type_idx));
    return _ret_num_tab[type_idx * VALTY_COUNT + valty];
  }
  template<VALTYPE valty>
  UINT32    Get_func_param_num(UINT32 func_idx) const {
    UINT32 proto_idx = Get_func(func_idx).Get_type_idx();
    return Get_proto_param_num<valty>(proto_idx);
  }
  template<VALTYPE valty>
  UINT32    Get_func_ret_num(UINT32 func_idx) const {
    UINT32 proto_idx = Get_func(func_idx).Get_type_idx();
    return Get_proto_ret_num<valty>(proto_idx);
  }

public:
  static void   Init_module_from_buffer(U_MODULE *module, char *buffer, UINT32 buffer_len);
  AM_EXPAND    *Map_to_expand_pc(UINT32 func_id, UINT32 ubh_pc) const;
  BOOL          Map_to_ubh_pc(AM_EXPAND *exp, UINT32 &func_id, UINT32 &ubh_pc) const;
  void          Bind_expand_pc(AM_EXPAND *exp, UINT32 func_id, UINT32 ubh_pc);
};


#endif //UWASM_VM_U_MODULE_H
