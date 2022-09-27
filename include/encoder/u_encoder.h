/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef U_ENCODER_H
#define U_ENCODER_H

#include <vector>
#include <string>
#include "ubh/ubh.h"

using namespace std;

class U_TYPE_ENCODER;
class U_CODE_BODY;
class U_INST_ENCODER;
class U_STR_TAB;

enum NTYPE {
  NTYPE_V,
  NTYPE_U1,
  NTYPE_I1,
  NTYPE_U2,
  NTYPE_I2,
  NTYPE_U4,
  NTYPE_I4,
  NTYPE_U8,
  NTYPE_I8,
  NTYPE_F4,
  NTYPE_F8,
  NTYPE_PTR,
  NTYPE_NUM,
};

class U_ENCODER {
private:
  UINT32                       _cur_func_idx;
  vector<UBH_IMPORT>           _import_tab;
  vector<UBH_IMP_NAT>          _imp_nat_tab;
  vector<UBH_EXPORT>           _export_tab;
  vector<UBH_MEM>              _mem_tab;
  U_TYPE_ENCODER              *_u_type_encoder;
  vector<UBH_FUNC>             _func_tab;
  vector<UBH_GLBL>             _glbl_tab;
  U_INST_ENCODER              *_u_inst_encoder;
  UINT8                       *_const_data;                 // used for 'data' section.
  UINT64                       _const_data_size;
  UINT32                      *_reloc_data;
  UINT64                       _reloc_data_size;
  U_STR_TAB                   *_module_string_tab;          // module string table, first name is module name
  U_STR_TAB                   *_global_name_tab;            // global name
  U_STR_TAB                   *_nn_tab;                     // non null terminated string table
private:
  U_INST_ENCODER              &U_inst_encoder()                       { return *_u_inst_encoder; }
  U_STR_TAB                   &Module_str_tab()                       { return *_module_string_tab; }
  U_STR_TAB                   &Global_name_tab()                      { return *_global_name_tab; }
  U_TYPE_ENCODER              &U_type_encoder()                       { return *_u_type_encoder; }
  vector<UBH_IMPORT>          &Import_tab()                           { return _import_tab; }
  vector<UBH_IMP_NAT>         &Imp_nat_tab()                          { return _imp_nat_tab; }
  vector<UBH_EXPORT>          &Export_tab()                           { return _export_tab; }
  vector<UBH_FUNC>            &Func_tab()                             { return _func_tab; }
  UINT8                       *Const_data()                           { return _const_data; }
  UINT64                       Const_data_size()                const { return _const_data_size; }
  UINT32                      *Reloc_data()                           { return _reloc_data; }
  UINT64                       Reloc_data_size()                const { return _reloc_data_size; }
  vector<UBH_GLBL>            &Glbl_tab()                             { return _glbl_tab; }
  U_STR_TAB                   &Nn_tab()                               { return *_nn_tab; }
  U_CODE_BODY                 &Cur_code_body();
  bool                         Pre_check();
  UINT32                       Inner_request_func(const char *func_name, UINT32 type_idx, UINT32 flags, bool &exist);
  void                         Add_to_import_tab(UINT32 func_idx);
  void                         Add_to_export_tab(UINT32 func_idx);
  UINT32                       Encode_and_save_ntype(NTYPE ret_ntype, vector<NTYPE> &param_ntype_vec);
  void                         Add_to_import_native_tab(UINT32 func_idx, UINT32 ntype_off);
  void                         Finish();
  void                         Collect_write_info(vector<pair<UINT32, UINT32> > &sec_id_vec, vector<UBH_SHDR> &ubh_shdr_vec, UINT32 *total_size);
  void                         Write_mhdr(char *buf, UINT32 shdr_num, UINT32 shdr_off);
  void                         Write_section(char *buf, vector<pair<UINT32, UINT32> > &sec_id_vec);
  void                         Write_shdr(char *buf, vector<UBH_SHDR> &ubh_shdr_vec);

public:
  U_ENCODER();
  ~U_ENCODER();
  void                         Set_data_section(UINT64 data_size, char *data_value);
  void                         Set_reloc_section(UINT64 reloc_size, UINT32 *reloc_value);
  UINT32                       Add_global(BOOL mutable_, UINT32 valty, UINT64 init_value, const char *sym_name);
  UINT32                       Add_global(BOOL mutable_, UINT32 init_value, const char *sym_name);
  UINT32                       Add_global(const char *sym_name);
  UINT32                       Start_func(UINT32 func_idx, UINT32 local_regs[4], UINT32 io_regs[4], UINT32 stack_size);
  UINT32                       Request_func(const char *func_name, UINT32 type_idx, UINT32 flags);
  UINT32                       Request_native_func(const char *func_name, UINT32 type_idx, NTYPE ret_ntype, vector<NTYPE> &param_ntype_list);
  UINT32                       Add_inst(UINT32 opcode, VALTYPE valtype, TY_AM ty_am, vector<UINT32> &content);
  UINT32                       Add_type(vector<VALTYPE> &ret_type_list, vector<VALTYPE> &param_type_list);
  UINT32                       Add_label(char *label_name);
  UINT32                       Request_label(const char *label_name);
  void                         Write_to_file(const char *file_path);
  U_CODE_BODY                 &Get_inst_encoder(UINT32 func_idx);
};
void UVM_set_trace(int on);
#endif //U_ENCODER_H
