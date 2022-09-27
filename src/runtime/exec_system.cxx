/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include "uvm/uvm.h"
#include "exec_system.h"
#include "utilities/u_utils.h"
#include "runtime/u_runtime.h"

// Printing the system status
void UWASM_SYSTEM::Print(FILE *f) {
  _machine.Print(f);
}

// Executing the function with continue.
void UWASM_SYSTEM::Exec_continue() {
  _machine.Exec_start();
}

// load module from buffer
U_MODULE *UWASM_SYSTEM::Load_from_buffer(char *buffer, UINT32 buffer_len) {
  U_MODULE &u_module = _machine.Get_u_module();
  U_MODULE::Init_module_from_buffer(&u_module, buffer, buffer_len);
  Is_Trace_cmd(Tracing(TP_BIN_READ), {
    u_module.Print(TFile);
  });
  return &u_module;
}

INT32 UWASM_SYSTEM::Search_start() {
  U_MODULE &module = Machine().Get_u_module();
  for (UINT32 i = 0; i < module.Mhdr().Get_shdr_num(); i++) {
    UBH_SHDR &shdr = module.Get_shdr(i);
    if (shdr.Get_sec_tag() == SEC_START) {
      UINT16 func_idx = shdr.Get_idx();
      UBH_FUNC &func = module.Get_func(func_idx);
      char *func_name = module.Get_module_name(func.Get_name_idx());
      Is_Trace(Tracing(TP_EXEC),
               (TFile, "Found start section header, set"
                       " start function, func inx: %d, func name: %s\n",
                 func_idx, func_name));
      return func_idx;
    }
  }


  Is_Trace(Tracing(TP_EXEC),
           (TFile, "Start section not founc, search for first "
                   "non-imported func instead\n"));
  // Find the first non-imported function instead.
  for (UINT32 i = 0; i < module.Mhdr().Get_shdr_num(); i++) {
    UBH_SHDR &shdr = module.Get_shdr(i);
    if (shdr.Get_sec_tag() == SEC_FUNC) {
      UINT16 func_cnt = shdr.Get_elem_num();
      for (UINT32 fid = 0; fid < func_cnt; fid++) {
        UBH_FUNC &func = module.Get_func(fid);
        if (func.Get_flags() & U_FUNC_FLAG::F_EXTERN ||
            func.Get_flags() & U_FUNC_FLAG::F_IMPORT_WASM) {
          continue;
        }
        return fid;
      }
    }
  }
  Is_True(false, ("No viable start function found."));
  // No function found, return 0 as backup.
  return 0;
}


// Search a function with given name
INT32 UWASM_SYSTEM::Search_start_by_name(const char *func_name) {
  U_MODULE &module = Machine().Get_u_module();

  // Find the first non-imported function instead.
  for (UINT32 i = 0; i < module.Mhdr().Get_shdr_num(); i++) {
    UBH_SHDR &shdr = module.Get_shdr(i);
    if (shdr.Get_sec_tag() == SEC_FUNC) {
      UINT16 func_cnt = shdr.Get_elem_num();
      for (UINT32 fid = 0; fid < func_cnt; fid++) {
        UBH_FUNC &func = module.Get_func(fid);
        if (strcmp(func_name, module.Get_module_name(func.Get_name_idx())) != 0) {
          Is_Trace(Tracing(TP_BIN_READ), (TFile, "Function skipped %s \n", module.Get_module_name(func.Get_name_idx())));
          continue;
        }
        if (func.Get_flags() & U_FUNC_FLAG::F_EXTERN ||
            func.Get_flags() & U_FUNC_FLAG::F_IMPORT_WASM) {
          Is_True_Rel(false, ("Start function given is imported."));
          continue;
        }
        return fid;
      }
      break;
    }
  }
  Is_True_Rel(false, ("No viable start function found. %s", func_name));
  // No function fo
  // und, return 0 as backup.
  return 0;
}

// Perform the 'next' operation in debugging
void UWASM_SYSTEM::Debug_next_instr(INT32 count) {
  // Printing the next instruction
  _machine.Control_start(count);
}

// load module from file
U_MODULE *UWASM_SYSTEM::Load_from_file(const char *filename) {
  // Testing in reading from an array.
  INT32 fd;
  char *buffer;
  UINT32 buffer_len;
  fd = Read_and_mmap(filename, &buffer, &buffer_len);
  if (fd < 0) {
    // File reading failed.
    fprintf(stderr, "Failed to read the uwm file '%s', please verify if the file is accessible.\n", filename);
    exit(EXIT_FAILURE);
  }
  Is_True(fd >= 0, ("Read file error."));
  U_MODULE *u_module = Load_from_buffer(buffer, buffer_len);
  U_RUNTIME::Init_cache(*u_module);
  return u_module;
}

MODULE_HANDLE UVM_load(char *buffer, uint32_t buffer_len) {
  U_MODULE *module = Uvm_system()->Load_from_buffer(buffer, buffer_len);
  return module;
}

uint32_t UVM_init_memory(MODULE_HANDLE mh, char *mem_p, uint32_t mem_size) {
  return Uvm_system()->Init_memory(mem_p, mem_size);
}

U_MODULE &UVM_get_module_by_handle(MODULE_HANDLE mod_handle) {
  return Uvm_system()->Machine().Get_u_module();
}

bool UVM_get_export_funcs(MODULE_HANDLE mh, EXPORT_FUNC **export_funcs, uint32_t *number) {
  // we don't wrote down the export section, so just export all function
  U_MODULE &module = UVM_get_module_by_handle(mh);
  *number = module.Get_shdr_by_sec_id(S_EXPORT).Get_elem_num();
  *export_funcs = (EXPORT_FUNC *) malloc(sizeof(EXPORT_FUNC) * (*number));
  Is_True(*export_funcs != NULL, ("memory out of space"));
  UINT32 string_tbl_base_ofst = (UINT32) (UINT64) ((UINT64) module.Module_name_sec() - (UINT64) &module.Mhdr());
  for (UINT32 i = 0; i < module.Get_shdr_by_sec_id(S_EXPORT).Get_elem_num(); i++) {
    UBH_EXPORT &export_entry = module.Get_export(i);
    UINT32 func_idx = export_entry.Get_func_idx();
    UBH_FUNC &func = module.Get_func(func_idx);
    EXPORT_FUNC &e_func = (*export_funcs)[i];
    e_func.name_offset = string_tbl_base_ofst + func.Get_name_idx();
    e_func.name_size = (uint32_t)strlen(module.Get_module_name(func.Get_name_idx()));
    e_func.func_index = func_idx;
    e_func.type_index = func.Get_type_idx();
    UBH_TYPE &m_type = module.Get_type(func.Get_type_idx());
    e_func.param_count = module.Get_param_num(func.Get_type_idx());
    e_func.return_count = m_type.Get_ret_num();
  }
  return true;
}

const char *UVM_get_func_name(MODULE_HANDLE mh, uint32_t name_offset) {
  return (const char *) UVM_get_module_by_handle(mh).Get_module_name(name_offset);
}

UVM_VALTY_ENUM UVM_get_param_type(MODULE_HANDLE mh, uint32_t func_index, uint16_t index) {
  U_MODULE &module = UVM_get_module_by_handle(mh);
  UBH_FUNC &func   = module.Get_func(func_index);
  return (UVM_VALTY_ENUM) module.Get_param_type(func.Get_type_idx(), index);
}

UVM_VALTY_ENUM UVM_get_return_type(MODULE_HANDLE mh, uint32_t func_index, uint16_t index) {
  U_MODULE &module = UVM_get_module_by_handle(mh);
  UBH_FUNC &func   = module.Get_func(func_index);
  return (UVM_VALTY_ENUM) module.Get_ret_type(func.Get_type_idx(), index);
}

void UVM_set_func(MODULE_HANDLE mh, uint32_t func_idx) {
  // TODO: CORRECTLY USE THE MODULE HANDLE.
  Uvm_system()->Machine().Init_env(func_idx);
}

void UVM_set_reg_i32(uint32_t reg, int32_t v) {
  Uvm_system()->Machine().Reg_i32().Set(reg, v);
}

void UVM_set_reg_i64(uint32_t reg, int64_t v) {
  Uvm_system()->Machine().Reg_i64().Set(reg, v);
}

void UVM_set_reg_f32(uint32_t reg, float v) {
  Uvm_system()->Machine().Reg_f32().Set(reg, v);
}

void UVM_set_reg_f64(uint32_t reg, double v) {
  Uvm_system()->Machine().Reg_f64().Set(reg, v);
}

uint32_t UVM_get_reg_i32(uint32_t reg) {
  return Uvm_system()->Machine().Reg_i32().Get(reg);
}

uint64_t UVM_get_reg_i64(uint32_t reg) {
  return Uvm_system()->Machine().Reg_i64().Get(reg);
}

float UVM_get_reg_f32(uint32_t reg) {
  return Uvm_system()->Machine().Reg_f32().Get(reg);
}

double UVM_get_reg_f64(uint32_t reg) {
  return Uvm_system()->Machine().Reg_f64().Get(reg);
}

void UVM_execute() {
  Uvm_system()->Exec_continue();
}

void UVM_free_module(MODULE_HANDLE mh) {
}

void UVM_destroy() {
  // TODO: need to de-allocate machine and module
}

UWASM_SYSTEM *Uvm_system() {
  static UWASM_SYSTEM system;
  return &system;
}
