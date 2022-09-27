/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include "runtime/exec_system.h"
#include "interpreter/exec_debug.h"
#include <iostream>
#include <cstdlib>
#include <cstdarg>
#include <map>
#include "runtime/u_runtime_reg.h"

using namespace std;

#define CMD_BUF_LEN      100


namespace UWASM_DEBUGGER {
  BOOL is_ready = false;

  enum OPTION {
    NEXT,
    CONTINUE,
    QUIT,
    TRACE,
    INTERACT,
    HELP,
    SET_REG,
    PRINT_REG,
    OPEN_FILE,
    SET_FUNC,
    PRINT,
    NO_TRACE,
    RUN_INSTR_CNT,
    CHECK_RET,
    SET_FUNC_NAME,
    ADD_RUN_ARG,
    ADD_STRING_ARG,
    BREAKPOINT_SET,
    BREAKPOINT_COND_SET,
    BREAKPOINT_DISABLE,
    WATCHPOINT_SET,
    EXAMINE_MEM,
    PRINT_PC,
    PRINT_FUNC,
    PRINT_CALLSTACK,
    CHANGE_CURRENT_FRAME,
    PRINT_CURRENT_FRAME,
    NOPT
  };

  class OPTION_PARSER {
  private:
    static map<string, OPTION> option_map;
  public:
    static void Add_option(OPTION opt_enum, int num, ...) {
      va_list opt_l;
      va_start(opt_l, num);
      char *opt;
      while (num--) {
        opt = va_arg(opt_l, char*);
        option_map.insert(pair<string, OPTION>(opt, opt_enum));
      }
      va_end(opt_l);
    }

    static OPTION Parse_option(const string& opt) {
      if (option_map.find(opt) == option_map.end()) return NOPT;
      return option_map.at(opt);
    }
  };

  map<string, OPTION> OPTION_PARSER::option_map;

  enum READ_STATUS{
    OK = 0,
    NON_OK = 1,
  };
}

#define CHECK_VM_READY() if (!is_ready) { \
  Is_Trace(Tracing(TP_EXEC), \
         (TFile, "Function not starter, use 'func 0xXXXX' to start exec some function\n")); \
  return; \
}


class MULTI_VALTY_VALUE {
  VALTYPE _kind;
  bool    _is_set;
  union {
    INT32   _i32_value;
    INT64   _i64_value;
    FLOAT32 _f32_value;
    FLOAT64 _f64_value;
  };
public:
  VALTYPE Get_kind() const { return _kind;  }
  INT32 Get_i32_value() const { return _i32_value;  }
  INT64 Get_i64_value() const { return _i64_value;  }
  FLOAT32 Get_f32_value() const { return _f32_value;  }
  FLOAT64 Get_f64_value() const { return _f64_value;  }
  void Set_kind(VALTYPE kind) { _kind = kind;  }
  void Set_i32_value(INT32 i32Value) { _i64_value = 0; _i32_value = i32Value; _is_set = true;  }
  void Set_i64_value(INT64 i64Value) { _i64_value = i64Value; _is_set = true; }
  void Set_f32_value(FLOAT32 f32Value) { _i64_value = 0;  _f32_value = f32Value; _is_set = true; }
  void Set_f64_value(FLOAT64 f64Value) { _f64_value = f64Value; _is_set = true; }
  MULTI_VALTY_VALUE() : _kind(VALTY_COUNT), _i64_value(0), _is_set(false) {}
  MULTI_VALTY_VALUE(INT32 val) : _kind(I32), _i32_value(val), _is_set(true) {}
  MULTI_VALTY_VALUE(INT64 val) : _kind(I64), _i64_value(val), _is_set(true) {}
  MULTI_VALTY_VALUE(FLOAT32 val) : _kind(F32), _f32_value(val), _is_set(true) {}
  MULTI_VALTY_VALUE(FLOAT64 val) : _kind(F64), _f64_value(val), _is_set(true) {}
  bool Is_set() { return _is_set; }
};

using namespace UWASM_DEBUGGER;

void Start_from_func(UINT32 func_id);

MULTI_VALTY_VALUE Read_cmd_line_value(char **argv, INT32 i);
MULTI_VALTY_VALUE Read_cmd_line_value(const char *arg1, const char *arg2);

void Check_return_value(UWASM_SYSTEM &system, MULTI_VALTY_VALUE &value);

void Set_value_from_argval(UWASM_SYSTEM &uvm_system, UINT32 *next_reg_to_set, const MULTI_VALTY_VALUE &value);

// Printing the help message
//
void Print_help_msg() {
  fprintf(TFile, DBAR "Usage:   uvm [-h] <..commands..> \n"
                 "Commands:  \n"
                 "     file <filepath>        - Specifying the file to execute\n"
                 "     func <func-id>         - Select a function for start\n"
                 "     sreg <reg-id> <value>  - Set a value to a register\n"
                 "     trace <kind> <level>   - Set a tracing option to a value\n"
                 "     interact 1             - Enable interactive mode \n" DBAR);
}


void Print_option_status() {
  fprintf(TFile, DBAR " Option status : BinRead=%d, Exec=%d, Eperf=%d\n",
          Tracing(TRACE_KIND::TP_BIN_READ),
          Tracing(TRACE_KIND::TP_EXEC),
          Tracing(TRACE_KIND::TP_EPERF));
}

READ_STATUS Parse_int_val(const char *str, UINT64 &value, BOOL is_func_id = false, UWASM_SYSTEM *system = nullptr) {
  INT32 cnt = 0;
  if (is_func_id && system != nullptr) {
    // Try to read as a func name first.
    UINT32 func_num = system->Machine().Get_u_module().Get_shdr_by_sec_id(S_FUNC).Get_elem_num();
    for (UINT32 i = 0; i < func_num; i++) {
      UINT32 func_name_idx = system->Machine().Get_u_module().Get_func(i).Get_name_idx();
      const char *func_name = system->Machine().Get_u_module().Get_module_name(func_name_idx);
      if (strlen(func_name) > 0 && strcmp(func_name, str) == 0) {
        value = i;
        return READ_STATUS::OK;
      }
    }
  }
  if (strncmp(str, "0x", 2) == 0) {
    // use hexadecimal format.
    INT32 res = 0;
    cnt = sscanf(str, "%x", &res);
    value = res;
  } else if (strncmp(str, "(uint32)", 8) == 0) {
    // use uint32 format.
    UINT32 res = 0;
    cnt = sscanf(str, "(uint32)%u", &res);
    value = res;
  } else {
    UINT32 res = 0;
    cnt = sscanf(str, "%d", &res);
    value = res;
  }
  return cnt > 0 ? OK : NON_OK;
}

void Parse_dbg_cmd(const char *str, UWASM_SYSTEM &system, UINT32 *next_reg_to_set) {
  char cmd_name[CMD_BUF_LEN];
  char *text_val1 = new char[1024];
  char *text_val2 = new char[1024];
  char *text_val3 = new char[1024];
  INT64 val1 = 0, val2 = 0, val3 = 0;
  INT32 ival1 = 0, ival2 = 0, ival3 = 0, ivalcnt = 0, textcnt = 0;
  UWASM_DEBUG &debug = Uvm_debug_tracker();
  if (sscanf(str, "%90s%llx%llx%llx", cmd_name, &val1, &val2, &val3) <= 0) {
    Is_Trace(Tracing(TP_EXEC),
             (TFile, "User didn't input any command."));
    return;
  }
  ivalcnt = sscanf(str, "%90s%d%d%d", cmd_name, &ival1, &ival2, &ival3);
  textcnt = sscanf(str, "%90s %1024s %1024s %1024s", cmd_name, text_val1, text_val2, text_val3);
  switch (OPTION_PARSER::Parse_option(cmd_name)) {
    case NEXT: {
      // Exec one instruction.
      CHECK_VM_READY();
      system.Debug_next_instr(1);
      break;
    }
    case CONTINUE: {
      CHECK_VM_READY();
      system.Exec_continue();
      break;
    }
    case QUIT: {
      exit(0);
    }
    case TRACE: {
      Set_tracing_opt((TRACE_KIND) val1, val2);
      break;
    }
    case HELP: {
      Print_help_msg();
      break;
    }
    case SET_REG: {
      Is_Trace(Tracing(TP_EXEC), (TFile, "Set R%03x = 0x%08x (%d) \n", (UINT32) val1, (UINT32) val2, (UINT32) val2));
      system.Debug_set_reg_i32(val1, val2);
      break;
    }
    case OPEN_FILE: {
      system.Load_from_file(text_val1);
      UINT32 mem_size = 1024 * 1024 * 128; // By default we are using 128M.
      char *mem_p = (char *) malloc(mem_size);
      system.Init_memory(mem_p, mem_size); // FIXME: should malloc in Init_memory
      break;
    }
    case SET_FUNC: {
      is_ready = true;
      system.Set_cur_func(val1);
      break;
    }
    case RUN_INSTR_CNT: {
      system.Debug_next_instr(ival1);
      break;
    }
    case ADD_RUN_ARG: {
      MULTI_VALTY_VALUE res = Read_cmd_line_value(text_val1, text_val2);
      Set_value_from_argval(system, next_reg_to_set, res);
      break;
    }
    case ADD_STRING_ARG: {
      system.Machine().Get_string_args().push_back(text_val1);
      break;
    }
    case BREAKPOINT_SET: {
      UINT64 func_id = 0, pc = 0;
      if (Parse_int_val(text_val1, func_id, true, &system) != READ_STATUS::OK ||
          Parse_int_val(text_val2, pc) != READ_STATUS::OK) {
        fprintf(TFile, "Value '%s' not recognized.\n", text_val1);
        break;
      }
      AM_EXPAND *exp = system.Machine().Get_u_module().Map_to_expand_pc(func_id, pc);
      if (exp == nullptr) {
        fprintf(TFile, "Func id = %llu, ubh-pc = %llu(0x%08llx) not found in cache.\n", func_id, pc, pc);
        break;
      }
      debug.Add_breakpoint(func_id, pc, (AM_EXPAND *) exp, 0);
      break;
    }
    case WATCHPOINT_SET: {
      UINT64 ofst;
      if (Parse_int_val(text_val1, ofst) != READ_STATUS::OK) {
        fprintf(TFile, "Value '%s' not recognized.\n", text_val1);
        break;
      }
      debug.Add_watchpoint(ofst, system.Machine().Mem_pointer() + ofst);
      break;
    }
    case PRINT: {
      debug.Print();
      break;
    }
    case PRINT_CALLSTACK: {
      // Current: system.Machine().Cur_env();
      // And all parents
      debug.Print_callstack();
      break;
    }
    case CHANGE_CURRENT_FRAME: {
      debug.Change_current_frame(ival1);
      break;
    }
    case PRINT_CURRENT_FRAME: {
      debug.Print_current_frame();
      break;
    }
    case PRINT_FUNC: {
      if (ivalcnt >= 2) {
        debug.Print_func(ival1);
      } else {
        debug.Print_current_func();
      }
      break;
    }
    default:
      fprintf(TFile, "Command not recognized.\n");
      break;
  }
}


bool Is_interactive_mode(INT32 argc, char **argv) {
  for (INT32 i = 0; i < argc; i++)
    if (OPTION_PARSER::Parse_option(argv[i]) == INTERACT) { return true; }
  return false;
}


void Start_vm_non_interactive(INT32 argc, char **argv) {
  UWASM_SYSTEM &uvm_system = *Uvm_system();
  if (argc == 2 && OPTION_PARSER::Parse_option(argv[1]) == HELP) {
    Print_help_msg();
    exit(0);
  }
  INT64                        start_func       = -1;
  vector<pair<UINT32, UINT32>> sreg;
  const char                   *start_func_name = nullptr;
  vector<MULTI_VALTY_VALUE>    arguments_list;
  vector<string>               arguments_list_string;
  MULTI_VALTY_VALUE            check_ret_value;
  const char                   *file_path       = nullptr;
  for (INT32 i = 1; i < (INT32) argc; i += 2) {
    const char *name = argv[i];
    const char *val  = argv[i + 1];
    switch (OPTION_PARSER::Parse_option(name)) {
      case OPEN_FILE: {
        file_path = val;
        break;
      }
      case TRACE: {
        // Set the first {val1} tracing options to be val2
        Set_tracing_opt((TRACE_KIND) strtol(val, NULL, 0),
                        strtol(argv[i + 2], NULL, 0));
        i++;
        break;
      }
      case SET_REG: {
        // Set a register
        sreg.emplace_back(strtol(val, NULL, 0),
                          strtol(argv[i + 2], NULL, 0));
        i++;
        break;
      }
      case SET_FUNC: {
        // Goto a specific function.
        start_func = strtol(val, NULL, 0);
        break;
      }
      case SET_FUNC_NAME: {
        start_func_name = val;
        break;
      }
      case CHECK_RET: {
        check_ret_value = Read_cmd_line_value(argv, i);
        i++;
        break;
      }
      case ADD_RUN_ARG: {
        MULTI_VALTY_VALUE value = Read_cmd_line_value(argv, i);
        arguments_list.push_back(value);
        i++;
        break;
      }
      case ADD_STRING_ARG: {
        string value = argv[i + 1];
        arguments_list_string.push_back(value);
        break;
      }
      case NO_TRACE: {
        Set_tracing_opt(TP_EXEC, TRACE_UNABLE_LEVEL);
        Set_tracing_opt(TP_BIN_READ, TRACE_UNABLE_LEVEL);
        Set_tracing_opt(TP_BIN_WRITE, TRACE_UNABLE_LEVEL);
        i--;
        break;
      }
      case HELP:
        Print_help_msg();
        exit(0);
      default:
        exit(0);
    }
  }
  Is_True(file_path != nullptr, ("File path is NULL."));
  if (file_path == nullptr) {
    fprintf(stderr, "Please specific file path.\n");
    exit(-1);
  }

  // Using a specific file.
  uvm_system.Load_from_file(file_path);
  UINT32 mem_size = 1024 * 1024 * 128;
  char *mem_p = (char *) malloc(mem_size);
  uvm_system.Init_memory(mem_p, mem_size);

  // if not specific entry function, select main
  if (start_func_name != nullptr) {
    start_func = Uvm_system()->Search_start_by_name(start_func_name);
  } else if (start_func < 0) {
    start_func = uvm_system.Search_start();
  }
  uvm_system.Set_cur_func(start_func); // will init machine fp

  uvm_system.Set_string_args(arguments_list_string);

  // set arguments by -arg [VALTY] [value] option
  UINT32 next_reg_to_set[VALTY_COUNT] = {CONST_REG_NUM + RET_REG_NUM,
                                         CONST_REG_NUM + RET_REG_NUM,
                                         CONST_REG_NUM + RET_REG_NUM,
                                         CONST_REG_NUM + RET_REG_NUM};
  for (UINT32 i = 0; i < arguments_list.size(); i++) {
    MULTI_VALTY_VALUE &value   = arguments_list.at(i);
    Set_value_from_argval(uvm_system, next_reg_to_set, value);
  }

  // set regs after initializaton
  for (auto & it : sreg)
    uvm_system.Debug_set_reg_i32(it.first, it.second);

  // continue exec
  uvm_system.Exec_continue();

  if (check_ret_value.Is_set()) {
    Check_return_value(uvm_system, check_ret_value);
  }
}

void Set_value_from_argval(UWASM_SYSTEM &uvm_system, UINT32 *next_reg_to_set, const MULTI_VALTY_VALUE &value) {
  INT32  kind     = value.Get_kind();
  UINT32 next_reg = next_reg_to_set[kind];
  switch (kind) {
    case I32:  { uvm_system.Debug_set_reg_i32(next_reg, value.Get_i32_value()); break; }
    case I64:  { uvm_system.Debug_set_reg_i64(next_reg, value.Get_i64_value()); break; }
    case F32:  { uvm_system.Debug_set_reg_f32(next_reg, value.Get_f32_value()); break; }
    case F64:  { uvm_system.Debug_set_reg_f64(next_reg, value.Get_f64_value()); break; }
  }
  next_reg_to_set[kind] ++;
}

// Used for spec-test, assert return value is expected.
//
void Check_return_value(UWASM_SYSTEM &system, MULTI_VALTY_VALUE &value) {
  Is_True_Rel(value.Is_set(), ("Return value assertion not set ?? "));
  MULTI_VALTY_VALUE return_value;
  return_value.Set_kind(value.Get_kind());
  switch (value.Get_kind()) {
    case I32: { return_value.Set_i32_value(system.Machine().Reg_i32().Get(0)); break; }
    case I64: { return_value.Set_i64_value(system.Machine().Reg_i64().Get(0)); break; }
    case F32: { return_value.Set_f32_value(system.Machine().Reg_f32().Get(0)); break; }
    case F64: { return_value.Set_f64_value(system.Machine().Reg_f64().Get(0)); break; }
    default: {
      Is_True_Rel(false, ("Incorrect valtype given : %d", value.Get_kind()));
    }
  }
  Is_True_Rel(return_value.Get_i64_value() == value.Get_i64_value(),
              ("Return value assertion failure, expected = 0x%016llx, actual = 0x%016llx",
                value.Get_i64_value(), return_value.Get_i64_value()));
}

// Read command line value into multi-valty-value.
//
MULTI_VALTY_VALUE Read_cmd_line_value(const char *arg1, const char *arg2) {
  INT32 kind = strtol(arg1, NULL, 0);
  const char *arg_value = arg2;
  MULTI_VALTY_VALUE val;
  val.Set_kind((VALTYPE) kind);
  switch (kind) {
    case I32: {
      UINT64 value = strtol(arg_value, NULL, 0);
      val.Set_i32_value(value);
      break;
    }
    case I64: {
      UINT64 value = strtol(arg_value, NULL, 0);
      val.Set_i64_value(value);
      break;
    }
    case F32: {
      FLOAT32 value = strtof(arg_value, NULL);
      val.Set_f32_value(value);
      break;
    }
    case F64: {
      FLOAT64 value = strtod(arg_value, NULL);
      val.Set_f64_value(value);
      break;
    }
    default: {
      Is_True_Rel(false, ("Incorrect valty given : %d", kind));
    }
  }
  return val;
}


// Read command line value into multi-valty-value.
//
MULTI_VALTY_VALUE Read_cmd_line_value(char **argv, INT32 i) {
  return Read_cmd_line_value(argv[i + 1], argv[i + 2]);
}

void Start_vm_interactive(INT32 argc, char **argv) {
  (void) argc;  // TODO: Ignoring this param for now.
  (void) argv;  // TODO: Ignoring this param for now.

  UWASM_SYSTEM system;
  Set_tracing_opt(TP_EXEC, TRACE_ENABLE_LEVEL);
  Set_tracing_opt(TP_BIN_READ, TRACE_ENABLE_LEVEL);
  Set_tracing_opt(TP_BIN_WRITE, TRACE_ENABLE_LEVEL);
  UINT32 next_reg_to_set[VALTY_COUNT] = {CONST_REG_NUM + RET_REG_NUM,
                                         CONST_REG_NUM + RET_REG_NUM,
                                         CONST_REG_NUM + RET_REG_NUM,
                                         CONST_REG_NUM + RET_REG_NUM};
  // Initialize debugger.
  UWASM_DEBUG &debug = Uvm_debug_tracker();
  debug.Init(&system.Machine());
  while (true) {
    cout << ">> " ;
    std::string str;
    std::getline(cin, str);
    Parse_dbg_cmd(str.c_str(), system, next_reg_to_set);
  }
}

int main(int argc, char **argv) {
  // add option here
  OPTION_PARSER::Add_option(NEXT, 1, "n");
  OPTION_PARSER::Add_option(CONTINUE, 1, "c");
  OPTION_PARSER::Add_option(QUIT, 3, "q", "quit", "exit");
  OPTION_PARSER::Add_option(TRACE, 4, "t", "trace", "-t", "--trace");
  OPTION_PARSER::Add_option(INTERACT, 2, "i", "-i");
  OPTION_PARSER::Add_option(HELP, 3, "h", "-h", "--help");
  OPTION_PARSER::Add_option(SET_REG, 2, "sreg", "-sreg");
  OPTION_PARSER::Add_option(RUN_INSTR_CNT, 5, "runcount", "rc", "rn", "runc", "runn");
  OPTION_PARSER::Add_option(PRINT_REG, 2, "preg", "-preg");
  OPTION_PARSER::Add_option(OPEN_FILE, 3, "file", "-f", "--file");
  OPTION_PARSER::Add_option(SET_FUNC, 2, "func", "-func");
  OPTION_PARSER::Add_option(SET_FUNC_NAME, 2, "func_name", "-func_name");
  OPTION_PARSER::Add_option(CHECK_RET, 2, "check_ret", "-check_ret");
  OPTION_PARSER::Add_option(ADD_RUN_ARG, 3, "arg", "-arg", "--arg");
  OPTION_PARSER::Add_option(ADD_STRING_ARG, 4, "sarg", "-s", "-sarg", "--sarg");
  OPTION_PARSER::Add_option(NO_TRACE, 2, "ntrace", "-nt");
  OPTION_PARSER::Add_option(PRINT_CALLSTACK, 2, "bt", "backtrace");
  OPTION_PARSER::Add_option(PRINT_CURRENT_FRAME, 2, "pf", "pframe");
  OPTION_PARSER::Add_option(CHANGE_CURRENT_FRAME, 2, "f", "frame");
  OPTION_PARSER::Add_option(PRINT, 2, "p", "print");

  // Entry point for debugger.
  bool is_interaceive_mode = Is_interactive_mode(argc, argv);
  if (is_interaceive_mode) {
    Start_vm_interactive(argc, argv);
  } else {
    Start_vm_non_interactive(argc, argv);
  }
}