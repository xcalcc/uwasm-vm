/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include "converter.h"
#include "winst.h"
#include "ubh/u_opcode.h"
#include "ubh/u_module.h"
#include "encoder/u_encoder.h"
#include "encoder/u_inst_encoder.h"
#include "utilities/u_utils.h"
#include "trace/u_trace.h"
#include "local_table.h"
#include "reader.h"
#include "ubh/ubh.h"
#include "op_enum.h"
#include "conv_map.h"
#include "ir.h"
#include <iterator>
#include <map>
#include <set>
#include <algorithm>    // std::reverse

// A Macro to control inlining behavior
#define INLINE inline

using uvm::LABEL_TAB;
using std::set;
using UINTSET = std::set<UINT32>;
static const bool ENABLE_FREE_REGISTER = true;

// Trace option info
CONV_TRACE_OPT_INFO conv_trace_info;

// Conversion trace option
//
bool Convtrace(TRACE_CONV_KIND conv_kind) {
  return conv_trace_info.Get(conv_kind) >= conv_trace_info.Level() ||
         conv_trace_info.Get(TCK_ANY) >= conv_trace_info.Level();
}

void Set_conv_trace(TRACE_CONV_KIND kind, UINT32 level) {
  conv_trace_info.Set(kind, level);
}

using namespace std;
using WABTENUM = wabt::Type::WABTENUM;

CONVERT_OPTIONS &Get_conv_opt() {
  static CONVERT_OPTIONS conv_options;
  return conv_options;
}

// Print the wasm instruction vector to FILE *file
//
void Print_winst_vec(WINSVEC *ins, FILE * file=stdout) {
  fprintf(file, "===== Printing wasm instruction vector =====\n");
  UINT32 ins_cnt = BLOCK_INSTR_BEGIN;
  for (UINT32 i = 0 ; i < ins->size(); i++, ins_cnt++) {
    WASM_INST &insp = (*ins)[i];
    insp.Print((BUFFER) insp.Pc(), ins_cnt, file);
  }
  fprintf(file, "===== End of wasm instruction vector =====\n");
}

void Print_stmt(WINST_STMT_GRAPH &graph, UINT32 cur_id, UINT32 level, FILE *file=TFile) {
  vector<string> names = {"DUMMY",
                          "WINST_STMT_BLOCK",
                          "WINST_STMT_ROOT_BLOCK",
                          "WINST_STMT_INSTR"};
  WINST_STMT &stmt = graph.Stmt_id(cur_id);
  Is_True(stmt.Get_id() == cur_id, ("Incorrect id"));
  string item_kind_str = stmt.Get_stmt_kind() < names.size() ?
                         names.at(stmt.Get_stmt_kind()) : "UNKNOWN";

  const char *spaces = "                                          ";
  level = level < 20 ? level : 20;
  fprintf(file, "%.*s%-12s [%-4d], %-4d(0x%04x), %-4lu, (%-7s) ",
          level, spaces,
          OPCODE_inst(stmt.Get_opcode())->Get_name(),
          cur_id,
          stmt.Get_stmt_begin_pc(), stmt.Get_stmt_begin_pc(),
          stmt.Get_children().size(),
          item_kind_str.c_str());
  if (stmt.Get_stmt_kind() == WINST_STMT_BLOCK) {
    fprintf(file, "ret %d, %d", stmt.Get_result_cnt(), stmt.Get_result_type());
  }
  fputc('\n', file);
  for (int i = 0; i < stmt.Get_children().size(); i++) {
    Print_stmt(graph, stmt.Get_children().at(i), level + 1, file);
  }
}

// Print stmts
void Print_stmts(WINST_STMT_GRAPH &graph, FILE *file=TFile) {
  fprintf(file, "===== Printing block hierarchy vector =====\n");
  fprintf(file, "= Opc,   Id,  Start,  Child,  KindName ===\n");
  Print_stmt(graph, 0, 0, file);
  fprintf(file, "===== End of block hierarchy vector =====\n");
}

// Get the target label by finding the outer-level block.
UINT32 Get_parent_stmt_for_branch(UINT32 ins_cnt, WINST_STMT_GRAPH &graph, UINT32 jump_cnt) {
  UINT32 jump_levels = jump_cnt + 1;
  UINT32 current_stmt_id = graph.Get_stmt_id_by_inst(ins_cnt);
  for (UINT32 i = 0; i < jump_levels; i++) {
    current_stmt_id = graph.Stmt_id(current_stmt_id).Get_parent();
    Is_True_Rel(current_stmt_id != 0,
                ("Cannot jump out of function. ins_cnt = %d, jump_cnt = %d",
                 ins_cnt, jump_cnt));
  }
  return current_stmt_id;
}

// Get opcode's ty_am.
//
TY_AM Get_opc_tyam(U_OPCODE opc) {
  return UWINST_TY_AM[opc];
}

void Print_wasm_map(map<UINT32, UINT32> &wasm_to_uwasm_map, U_ENCODER &encoder, UINT32 func_id, WINSVEC &instrs, FILE *file) {
  encoder.Get_inst_encoder(func_id).Print_header(file);
  const UBH_CODE &c = encoder.Get_inst_encoder(func_id).Ubh_code();
  // print code info
  fprintf(file, ";; Local register num:\t(i32: %d, i64: %d, f32: %d, f64: %d)\n",
          c.Get_i32_local(), c.Get_i64_local(), c.Get_f32_local(), c.Get_f64_local());
  fprintf(file, ";; IO register num:\t\t\t(i32: %d, i64: %d, f32: %d, f64: %d)\n",
          c.Get_i32_io(), c.Get_i64_io(), c.Get_f32_io(), c.Get_f64_io());
  fprintf(file, ";; All register num:\t\t(i32: %d, i64: %d, f32: %d, f64: %d)\n",
          c.Get_i32_local() + c.Get_i32_io(), c.Get_i64_local() + c.Get_i64_io(),
          c.Get_f32_local() + c.Get_f32_io(), c.Get_f64_local() + c.Get_f64_io());
  fprintf(file, ";; Stack size: %d\n", c.Get_stack_size());
  fprintf(file, ";; Code sz: %d, label number: %d\n", c.Get_code_size(), c.Get_label_num());

  // Print in wasm mode.
  auto wasm_inst = instrs.begin();
  UINT32 *inst_tab = c.Get_inst_tab();
  UINT32 *vec = new UINT32[c.Get_code_size()];
  for (UINT32 i = 0; i < c.Get_code_size(); i++) {
    vec[i] = 0;
  }
  for (const auto &elem: wasm_to_uwasm_map) {
    vec[elem.second] = elem.first + 1;
  }
  for (UINT32 i = 0; i < c.Get_code_size(); i++) {
    if (vec[i] == 0) {
      // Not found.
      // Do nothing.
    }
  }
  // print instruction table
  fprintf(file, "\n");
  if (c.Get_code_size() > 0) {
    U_INST_ITER end(inst_tab + c.Get_code_size());
    for (U_INST_ITER iter(inst_tab); iter != end; iter++) {
      UINT32 ofst = iter.Addr() - inst_tab;
      if (vec[ofst] != 0) {
        fprintf(file, "%-50s#  WASM [%d]  #   ", " ", vec[ofst] - 1 - BLOCK_INSTR_BEGIN);
        Is_True((vec[ofst] - 1 - BLOCK_INSTR_BEGIN) < instrs.size(),
                ("Out of bound. wasm ins_cnt = %d, size = %lu",
                 vec[ofst] - 1 - BLOCK_INSTR_BEGIN, instrs.size()));
        instrs.at(vec[ofst] - 1 - BLOCK_INSTR_BEGIN).Print(file);
      }
      fprintf(file, "[%04d]\t\t", ofst);
      pair<bool, UINT64> inst_pair = *iter;
      if (inst_pair.first) {
        U_INST32 inst(inst_pair.second);
        inst.Print(file);
      } else {
        U_INST64 inst(inst_pair.second);
        inst.Print(file);
      }
    }
  } else {
    fprintf(file, "No instruction.\n");
  }
  // print label table
  fprintf(file, "\n[LBL]\n");
  UINT32 *label = c.Get_label_tab();
  if (c.Get_label_num() == 0) fprintf(file, "empty table\n");
  for (UINT32 i = 0; i < c.Get_label_num(); i++)
    fprintf(file, "%04d: %04d\n", i, label[i]);
}

// Print the wasm_stack, used only for debugging
//
void Print_stack(WASMSTK &stk, FILE *file = TFile) {
  WASMSTK temp;
  fprintf(file, "======== Print wasm stack ========\n"
                "Seq  Kind  Valty RegIdx  PushStmt \n");
  UINT32 stk_size = stk.size();
  for (UINT32 i = 0; i < stk_size; i++) {
    WASM_STACK_ELEM &elem = stk.top();
    fprintf(file, "%-4d ", i);
    elem.Print(file);
    temp.push(elem);
    stk.pop();
  }
  for (UINT32 i = 0; i < stk_size; i++) {
    WASM_STACK_ELEM &elem = temp.top();
    stk.push(elem);
    temp.pop();
  }
  fprintf(file, "======== End wasm stack ========\n");
}

// Print the wasm_stack, used only for debugging
//
void Print_stack(vector<WASM_STACK_ELEM> &stk, FILE *file = TFile) {
  WASMSTK temp;
  fprintf(file, "======== Print wasm stack ========\n"
                "Seq  Kind  Valty RegIdx  PushStmt \n");
  UINT32 stk_size = stk.size();
  for (UINT32 i = 0; i < stk_size; i++) {
    WASM_STACK_ELEM &elem = stk.at(i);
    fprintf(file, "%-4d ", i);
    elem.Print(file);
    temp.push(elem);
  }
  fprintf(file, "======== End wasm stack ========\n");
}


// Assert that the valtype of rs should be as desired
//
void Check_stack_elem_valty(UINT32 ins_cnt, WASMSTK & wasm_stack,
                            WASM_STACK_ELEM &rs,
                            UINT32 desired) {
  if (rs.Get_val_type() != desired) {
    Print_stack(wasm_stack);
    fprintf(stderr, "Rs: "); rs.Print(stderr);
  }
  Is_True_Rel(rs.Get_val_type() == desired,
              ("Valtype for an operand of inst should be %d, yet it is = %d, at inst = %d",
                desired, rs.Get_val_type(), ins_cnt));
}


// Assert that the valtype of rs should be same as rt.
//
void Check_stack_elem_valty(UINT32 ins_cnt, WASMSTK & wasm_stack,
                            WASM_STACK_ELEM &rs,
                            WASM_STACK_ELEM &rt) {
  if (rs.Get_val_type() != rt.Get_val_type()) {
    Print_stack(wasm_stack);
    fprintf(stderr, "Rs: "); rs.Print(stderr);
    fprintf(stderr, "Rt: "); rt.Print(stderr);
  }
  Is_True_Rel(rs.Get_val_type() == rt.Get_val_type(),
              ("Valtype for an operand of inst should be %d, yet it is = %d, inst = %d",
                rs.Get_val_type(), rt.Get_val_type(), ins_cnt));
}

// build block hierarchy by loop against loop and blocks
//
WINST_STMT_GRAPH &Build_block_hierarchy(U_ENCODER &encoder, WINSVEC &ins, LABEL_TAB &labels, WINST_STMT_GRAPH &graph) {
  // The current traversal hierarchy level,
  // push when creating / entering new block,
  // pop when end.
  stack<UINT32> work;
  UINT32 ins_cnt = BLOCK_INSTR_BEGIN;
  if (Convtrace(TCK_BLOCK)) {
    Print_winst_vec(&ins, TFile);
  }

  // Push a placeholder, to let all block id > 0.
  // REMOVE: blocks.push_back(WINST_BLK(0, BLK_NONE, 0));
  labels.Add(0); // placeholder as well.

  // Push a function block, to let all the next level blocks belong to it.
  Create_new_block(encoder, work, graph, WINST_OPCODE::Unreachable, VALTY_COUNT + 1, ins_cnt, BLK_FUNC);
  Is_Trace(Convtrace(TCK_BLOCK),
           (TFile, "====== Starting to build block hierarchy ======\n"));
  for (WINSITER it = ins.begin(); it != ins.end(); it++) {
    WASM_INST &inst = *it;
    INT32 block_type = wabt::Type::Void;
    switch (inst.Encode()) {
      case If:
      case Block:
      case Loop:
      {
        // Read bt and block type
        INT64 valty = Read_leb128((CONST_BUFFER) (inst.Pc() + 1));
        block_type = valty;
      }
      default : {
        // nothing;
      }
    }
    switch(inst.Encode()) {
      case If: {
        UINT32 stmt_id = Create_new_block(encoder, work, graph, inst.Encode(), block_type, ins_cnt, BLK_IF);
        Is_Trace(Convtrace(TCK_BLOCK),
                 (TFile, "Found a if id = %d, pc = 0x%08x\n", stmt_id, ins_cnt));
        break;
      }
      case (Block): {
        // push
        UINT32 stmt_id = Create_new_block(encoder, work, graph, inst.Encode(), block_type, ins_cnt, BLK_BLOCK);
        Is_Trace(Convtrace(TCK_BLOCK),
                 (TFile, "Found a block id = 0x%08x, pc = 0x%08x\n", stmt_id, ins_cnt));
        break;
      }
      case (Loop): {
        // push
        UINT32 stmt_id = Create_new_block(encoder, work, graph, inst.Encode(), block_type, ins_cnt, BLK_LOOP);
        Is_Trace(Convtrace(TCK_BLOCK),
                 (TFile, "Found a loop id = 0x%08x, pc = 0x%08x\n", stmt_id, ins_cnt));
        break;
      }
      case (Else): {
        // Pop_wasm_stk the if block
        Is_True(!work.empty(), ("Work list cannot be empty."));
        // REMOVE: UINT32 old_block_id = Finish_old_block(work, blocks, ins_cnt);
        UINT32 old_stmt_id  = Finish_old_block(work, ins_cnt);
        INT64  if_wabt_ty   = 0x80 + Conv_ty_wabt(graph.Stmt_id(old_stmt_id).Get_result_type());
        UINT32 else_id      = Create_new_block(encoder, work, graph, inst.Encode(), if_wabt_ty,
                                               ins_cnt, BLK_ELSE);
        WINST_STMT &if_stmt = graph.Stmt_id(old_stmt_id);
        WINST_STMT &else_stmt = graph.Stmt_id(else_id);
        if_stmt.Set_else_stmt(else_stmt.Get_id());
        else_stmt.Set_else_stmt(if_stmt.Get_id());
        else_stmt.Set_result_cnt(if_stmt.Get_result_cnt());
        else_stmt.Set_result_type(if_stmt.Get_result_type());
        Is_True(if_stmt.Get_opcode() == If,
                ("For dealing with 'else' stmt, the top of stack should be"
                 " a if-block, but it is %d at %d",
                 if_stmt.Get_opcode(), if_stmt.Get_id()));
        Is_Trace(Convtrace(TCK_BLOCK),
                 (TFile, "Found a else = 0x%08x, if_stmt = 0x%08x, pc = 0x%08x\n",
                   else_id, old_stmt_id, ins_cnt));
        break;
      }
      case (End): {
        // pop and set the values.
        if (work.empty()) {
          Is_True_Rel(false, ("End processed with emtpy work stack, error ?\n"));
          continue;
        }
        const WINST_STMT &x = WINST_STMT(WINST_STMT_KIND::WINST_STMT_INSTR, ins_cnt, work.top(), inst.Encode());
        UINT32 id = graph.Add_stmt_to_parent(x, ins_cnt, work.top());
        UINT32 par_stmt_id = Finish_old_block(work, ins_cnt);
        WINST_STMT &par_stmt = graph.Stmt_id(par_stmt_id);
        Is_Trace(Convtrace(TCK_BLOCK),
                 (TFile, "End of a block-level = (kind = %d, stmt-id = 0x%08x), pc = 0x%08x\n",
                   par_stmt.Get_opcode(), par_stmt_id, ins_cnt));
        break;
      }
      default : {
        const WINST_STMT &x = WINST_STMT(WINST_STMT_KIND::WINST_STMT_INSTR, ins_cnt, work.top(), inst.Encode());
        UINT32 id = graph.Add_stmt_to_parent(x, ins_cnt, work.top());
        break;
      }
    }
    ins_cnt ++;
  }
  Is_Trace_cmd(Convtrace(TCK_STMTS_GRAPH), { Print_stmts(graph, TFile); } );
}


UINT32 Finish_old_block(stack<UINT32> &work, UINT32 pc) {
  Is_True(!work.empty(), ("The work list should not be empty."));
  UINT32 tos_blk = work.top(); // top of stack block.
  work.pop();
  return tos_blk;
}


UINT32 Create_new_block(U_ENCODER &encoder,
                        stack<UINT32> &work,
                        WINST_STMT_GRAPH &graph,
                        UINT32 opc,
                        UINT32 wasm_ty,
                        UINT32 ins_cnt,
                        BLK_KIND kind) {
  // Find the previous sibling in this K-tree.
  UINT32 parent       = 0;

  UINT32 stmt_id = 0;
  if (!work.empty()){
    // Find parent.
    parent = work.top();
    // Find prev_sibling
    const WINST_STMT &x = WINST_STMT(WINST_STMT_KIND::WINST_STMT_BLOCK, ins_cnt, 0, opc);
    stmt_id = graph.Add_stmt_to_parent(x, ins_cnt, parent);
    if (opc == Loop || opc == Else) {
      graph.Stmt_id(stmt_id).Set_begin_label(Add_label(encoder, true, stmt_id, false));
    }
    UINT32 end_label_id = Add_label(encoder, false, stmt_id, false);
    graph.Stmt_id(stmt_id).Set_end_label(end_label_id);
    INT16 wasm_ty_num = wasm_ty;
    wasm_ty_num -= 0x80;
    if (wasm_ty_num != wabt::Type::Void && wasm_ty_num != -0x80) {
      graph.Stmt_id(stmt_id).Set_result_cnt(1);
      graph.Stmt_id(stmt_id).Set_result_type(Conv_wabt_ty(wasm_ty_num));
    } else if ((wasm_ty_num + 0x80) == (wabt::Type::Any)) {
      // This is abnormal.
      Is_True_Warn(false,
                   ("NYI, block type = any is not processed, "
                    "and may cause error. at inst = %d", ins_cnt));
      graph.Stmt_id(stmt_id).Set_result_cnt(0);
    } else {
      graph.Stmt_id(stmt_id).Set_result_cnt(0);
    }
  } else {
    const WINST_STMT &x = WINST_STMT(WINST_STMT_KIND::WINST_STMT_ROOT_BLOCK, ins_cnt, 0, opc);
    stmt_id = graph.Add_stmt(x, ins_cnt);
  }
  work.push(stmt_id);
  return stmt_id;
}

WASM_STACK_ELEM Pop_wasm_stk(REG_ALLOC_HELPER &reg_alloc,
                             stack<WASM_STACK_ELEM>& wasm_stack,
                             VALTYPE expect, UINT32 ins_cnt) {
  Is_True(wasm_stack.size() > 0, ("empty wasm_stack when popping"));
  // actual
  WASM_STACK_ELEM elem = wasm_stack.top();
  wasm_stack.pop();
  if(elem.Get_kind() == TEMP) {
    // Only free the register when it's a temp,
    // Don't do anything if it's local or global.
    if (ENABLE_FREE_REGISTER) {
      reg_alloc.Free_register(elem.Get_val_type(), elem.Get_idx());
    }
  } else if (elem.Get_kind() == LOCAL_KIND::UNREACHABLE_VALUE) {
    Is_True_Warn(false, ("Unreachable value extracted from wasm_stack."));
    return WASM_STACK_ELEM(LOCAL_KIND::UNREACHABLE_VALUE, expect, 0, elem.Get_push_stmt());
  }
  if (elem.Get_val_type() == VALTY_UNKNOWN) {
    return WASM_STACK_ELEM(LOCAL_KIND::UNREACHABLE_VALUE, expect, 0, elem.Get_push_stmt());
  } else if (expect == VALTY_UNKNOWN) {
    return elem;
  } else {
    Check_stack_elem_valty(0, wasm_stack, elem, expect);
  }
  return elem;
}


void Print_func_sig(wabt::FuncSignature *signature, FILE *file=TFile) {
  if (signature == nullptr) {
    fprintf(file, "nullptr\n");
    return;
  }
  std::vector<std::string> params_names = {"i32", "i64", "f32", "f64"};
  fprintf(file, "Func Sig == (");
  for (UINT32 i = 0; i < signature->GetNumParams(); i++) {
    fprintf(file, "%s", params_names.at(Conv_wabt_ty(signature->GetParamType(i))).c_str());
    if (i != signature->GetNumParams() - 1) {
      fputc(',', file);
    }
  }
  const char *ret_type = "void";
  if (signature->GetNumResults() > 0) {
    ret_type = params_names.at(Conv_wabt_ty(signature->GetResultType(0))).c_str();
  }
  fprintf(file, ") -> %s", ret_type);
  fputc('\n', file);
}


// Pop redundant values when exiting blocks.
void Pop_redundant_values(WINST_STMT &stmt, REG_ALLOC_HELPER &reg_alloc, WASMSTK &stack) {
  UINT32 stack_size = stack.size();
  UINT32 original_size = stmt.Get_stack_size();
  if (stack_size < original_size) {
    // Ah oh.
    Is_True_Rel(false,
                ("There should not be cases where block has "
                 "consumed values outside the block at end of stmt=%d \n"
                 "stack_size=%d, original_size=%d",
                  stmt.Get_id(), stack_size, original_size));
  }
  switch(stmt.Get_opcode()) {
    case Block:
    case Loop: {
      if (stack_size > original_size) {
        // To be popped.
        Is_True_Warn(false,
                     ("Excessive stack element to be popped on %d, "
                      "stack now = %d, should be = %d",
                       stmt.Get_id(), stack_size, original_size));
        for (UINT32 i = 0; i < stack_size - original_size; i++) {
          WASM_STACK_ELEM elem = Pop_wasm_stk(reg_alloc, stack, VALTY_UNKNOWN, stmt.Get_stmt_begin_pc());
          elem.Print(TFile);
        }
      }
      break;
    }
    case If: {
      if (stmt.Has_else()) {
        return;
      }
      // fallthrough expected.
    }
    case Else: {
      if (stack_size > original_size) {
        Is_True_Warn(false,
                     ("Excessive stack element to be popped on %d, "
                      "stack now = %d, should be = %d",
                       stmt.Get_id(), stack_size, original_size));
        for (UINT32 i = 0; i < stack_size - original_size; i++) {
          WASM_STACK_ELEM elem = Pop_wasm_stk(reg_alloc, stack, VALTY_UNKNOWN, stmt.Get_stmt_begin_pc());
          elem.Print(TFile);
        }
      }
      break;
    }
    default: {
      Is_True_Rel(false, ("NYI in Pop_redudant_values."));
    }
  }
}


// Push a value to the slot in the virtual stack,
// To get the register index to be used in the corresponding uwasm instruction.
// @param valtype
// @param pc
// @param wasm_stack The stack model
// @param local_tab  The local tables for each valtype
// @param local_map  A wasm-local-id to local-table index mapping.
// @return the register index to be used.
//
UINT32 Push_temp_reg(UINT32 ins_cnt, VALTYPE valtype, UINT64 pc, WASMSTK &wasm_stack,
                     LOCAL_TAB local_tab[], REG_ALLOC_HELPER &reg_alloc) {
  //ubh_code.Push_local_table(valtype, TEMP, pc);
  //UINT32 next = (UINT32) ubh_code.Get_table_num(valtype);
  UINT32 next = reg_alloc.Get_available_reg(valtype); // local_tab[valtype].size() + RET_REG_NUM;
  if (next >= (local_tab[valtype].size() + RET_REG_NUM)) {
    LOCAL_TAB_ELEM one(LOCAL_KIND::TEMP, valtype, ins_cnt);
    local_tab[valtype].push_back(one);
    Is_True(local_tab[valtype].size() + RET_REG_NUM > next,
            ("Not enought item pushed. valty = %d, local_tab = %d, reg_id = %d",
            valtype, local_tab[valtype].size(), next));
  }
  wasm_stack.push(WASM_STACK_ELEM(TEMP, valtype, next, ins_cnt));
  // FIXME: Local map should distinguish between different valtype.
  // local_map is useless when dealing with temps, cause there will be different local_map for each valtype.
  // local_map.push_back(LOCAL_MAP_ELEM(next, valtype));
  Is_True_Rel(next < 128, ("Register out of bound = %d", next));
  return next;
}

UINT32 Allocate_register(VALTYPE valtype, UINT64 pc, WASMSTK &wasm_stack,
                         LOCAL_TAB local_tab[], REG_ALLOC_HELPER &reg_alloc) {
  UINT32 next = reg_alloc.Get_available_reg(valtype);
  if (next >= (local_tab[valtype].size() + RET_REG_NUM)) {
    LOCAL_TAB_ELEM one(LOCAL_KIND::TEMP, valtype, pc);
    local_tab[valtype].push_back(one);
    Is_True(local_tab[valtype].size() + RET_REG_NUM > next,
            ("Not enought item pushed. valty = %d, local_tab = %d, reg_id = %d",
              valtype, local_tab[valtype].size(), next));
  }
  return next;
}

UINT32 Push_unreachable_value(WASMSTK &wasm_stack, UINT32 pc) {
  wasm_stack.push(WASM_STACK_ELEM(LOCAL_KIND::UNREACHABLE_VALUE, I32, 0, pc));
  return 0;
}

UINT32 Push_result_to_stack(WINST_STMT &stmt, WASMSTK &wasm_stack) {
  Is_True_Rel(stmt.Get_stmt_kind() == WINST_STMT_BLOCK,
              ("Not a block to be processed. = %d", stmt.Get_opcode()));
  if (stmt.Get_result_cnt() > 0) {
    Is_True_Rel(stmt.Get_result_reg() != 0,
                ("Block's result register should not be zero, at end of stmt = %d",
                  stmt.Get_id()));
    wasm_stack.push(WASM_STACK_ELEM(LOCAL_KIND::TEMP,
                                    static_cast<VALTYPE>(stmt.Get_result_type()),
                                    stmt.Get_result_reg(),
                                    stmt.Get_stmt_begin_pc()));
  }
}


INLINE LOCAL_IDX Get_tab_idx_by_reg_idx(UINT32 r) {
  return (LOCAL_IDX) (r - CONST_REG_NUM - RET_REG_NUM);
}

UINT32 Conv_func_idx(UINT32 wasm_func_idx) {
  return wasm_func_idx;
}

UINT32 Conv_type_idx(UINT32 wasm_type_idx, U_ENCODER &encoder, wabt::Module *module) {
  wabt::FuncType *funcType = module->GetFuncType(wabt::Var(wasm_type_idx));
  UINT32 flag     = 0;
  vector<VALTYPE> ret_list;
  vector<VALTYPE> parm_list;

  wabt::Index param_num = funcType->GetNumParams();
  // Use local_map as an intermediate storage for local-id -> local elem .
  // Deal with params
  for (wabt::Index i = 0; i < param_num; i++) {
    VALTYPE valtype = Conv_wabt_ty(funcType->GetParamType(i));
    Is_True(valtype >= VALTY_MIN && valtype < VALTY_COUNT,
            ("Incorrect valtype : %d", valtype));
    parm_list.push_back(valtype);
    LOCAL_TAB_ELEM tab_elem(LOCAL_KIND::FORMAL_PARAM, valtype, (UINT64) 0);
  }

  // Return types.
  for (wabt::Index i = 0; i < funcType->GetNumResults(); i++) {
    ret_list.push_back(Conv_wabt_ty(funcType->GetResultType(i)));
  }

  // add type to type section
  UINT32 type_idx = encoder.Add_type(ret_list, parm_list);
  return type_idx;
}

/**
 * read param and local from wasm
 * fill: local table, type section and function section
 * @return code_id
 */
UINT32 Create_ubh_code(wabt::Func* func, U_ENCODER &encoder,
                       LOCAL_TAB local_tab[],
                       LOCAL_MAP &local_map,
                       REG_ALLOC_HELPER& reg_helper,
                       FILE_CONTEXT &ctx) {
  UINT32 flag     = 0;
  vector<VALTYPE> ret_list;
  vector<VALTYPE> parm_list;
  wabt::Index param_num = func->GetNumParams();
  wabt::Index local_num = func->GetNumLocals();
  UINT32 local_num_for_type[VALTY_COUNT] = {0, 0, 0, 0};
  UINT32 io_regs_max_size[VALTY_COUNT] = {0};

  // Use local_map as an intermediate storage for local-id -> local elem .
  // Deal with params
  for (wabt::Index i = 0; i < param_num; i++) {
    VALTYPE valtype = Conv_wabt_ty(func->GetParamType(i));
    Is_True(valtype >= VALTY_MIN && valtype < VALTY_COUNT,
            ("Incorrect valtype : %d", valtype));
    parm_list.push_back(valtype);
    LOCAL_TAB_ELEM tab_elem(LOCAL_KIND::FORMAL_PARAM, valtype, (UINT64) 0);
    local_tab[valtype].push_back(tab_elem);
    local_map.push_back(LOCAL_MAP_ELEM((REG_IDX) local_tab[valtype].size(),
                                       valtype, LOCAL_KIND::FORMAL_PARAM));
  }

  // Deal with code-locals
  for (wabt::Index i = param_num; i < param_num + local_num; i++) {
    wabt::Type local = func->GetLocalType(i);
    VALTYPE    ty    = Conv_wabt_ty(local);
    LOCAL_TAB_ELEM tab_elem(LOCAL_KIND::LOCAL, ty, (UINT64) 0);
    local_tab[ty].push_back(tab_elem);
    local_map.push_back(LOCAL_MAP_ELEM((REG_IDX) local_tab[ty].size(), ty, LOCAL_KIND::LOCAL));
  }

  // Recalculate numbers
  for (UINT32 i = VALTY_MIN; i < VALTY_COUNT; i++) {
    // this will be overriden later in the code filling.
    // The number 100 is only to assert the correctness.
    io_regs_max_size[i] = 0; // Should be calculated later.
    local_num_for_type[i] = local_tab[i].size() + RET_REG_NUM;
    reg_helper.Preoccupy_registers((VALTYPE) i, RET_REG_NUM, local_tab[i].size());
  }

  // Return types.
  for (wabt::Index i = 0; i < func->GetNumResults(); i++) {
    ret_list.push_back(Conv_wabt_ty(func->GetResultType(i)));
  }

  // add type to type section
  UINT32 type_idx = encoder.Add_type(ret_list, parm_list);

  std::string cur_func_name = func->name;
  if (ctx.Get_func_name_cache(ctx.Get_func_idx()) != nullptr) {
    cur_func_name = ctx.Get_func_name_cache(ctx.Get_func_idx());
  } else if (func->name.empty()) {
    cur_func_name = ctx.Get_anon_func_name();
  }

  // Flag should be zero because it is neither native nor extern.
  flag = 0;

  // Start function
  UINT32 func_idx = encoder.Request_func((char *) cur_func_name.c_str(), type_idx, flag);
  encoder.Start_func(func_idx, local_num_for_type, io_regs_max_size, 0);
  Is_Trace(Convtrace(TCK_UBH),
           (TFile, "After Create_ubh_func, we have the follow status:\n"
            "Local Tables Sizes: [0] => %d, [1] => %d, [2] => %d, [3] => %d\n"
            "Local Map Sizes   :     => %lu \n",
             local_tab[0].size(), local_tab[1].size(),
             local_tab[2].size(), local_tab[3].size(),
             local_map.size()));
  return func_idx;
}

UINT32 Add_label(U_ENCODER &encoder, BOOL begin, UINT32 pc, BOOL add_new_label) {
  char label_name[1024];
  if (begin) {
    sprintf(label_name, ".L_begin_%u", pc);
  } else {
    sprintf(label_name, ".L_end_%u", pc);
  }
  UINT32 id = 0;
  if (add_new_label) {
    Is_Trace(Convtrace(TCK_LABEL),
             (TFile, "[Label] Add a label %.20s, at pc = %u, id = %d \n",
               label_name, pc, id));
    id = encoder.Add_label(label_name);
  } else {
    Is_Trace(Convtrace(TCK_LABEL),
             (TFile, "[Label] Request a label %.20s, at pc = %u, id = %d \n",
               label_name, pc, id));
    id = encoder.Request_label(label_name);
  }
  return id;
}

LOCAL_MAP_ELEM &Get_wasm_local_ty(LOCAL_MAP &wasm_local_ty_map, LOCAL_IDX idx) {
  Is_True(wasm_local_ty_map.size() > idx, ("local map doesn't contain entry with idx = %d.", idx));
  return wasm_local_ty_map.at(idx);
}

// Find the label id for a jump stmt.
UINT32 Get_label_id_for_jump(UINT32 ins_cnt, UINT32 jump_cnt, WINST_STMT_GRAPH &graph) {
  UINT32 target_blk_stmt = Get_parent_stmt_for_branch(ins_cnt, graph, jump_cnt);
  if (graph.Stmt_id(target_blk_stmt).Get_opcode() == Loop) {
    return graph.Stmt_id(target_blk_stmt).Get_begin_label();
  } else if (graph.Stmt_id(target_blk_stmt).Get_opcode() == If && graph.Stmt_id(target_blk_stmt).Has_else()) {
    // If there is an else block, use else block's end.
    return graph.Stmt_id(graph.Stmt_id(target_blk_stmt).Get_else_stmt()).Get_begin_label();
  } else {
    return graph.Stmt_id(target_blk_stmt).Get_end_label();
  }
}

// Add a mov instruction before break/else instruction
// return # of items popped from stack.
UINT32
Add_mov_for_break(U_ENCODER &encoder,
                  WINST_STMT_GRAPH &graph,
                  UINT32 target_blk_stmtid,
                  REG_ALLOC_HELPER &reg_helper,
                  WASMSTK &wasm_stack,
                  UINT32 ins_cnt,
                  BOOL pop_stack) {
  WINST_STMT &target_stmt = graph.Stmt_id(target_blk_stmtid);
  if (target_stmt.Get_result_cnt() > 0) {
    WASM_STACK_ELEM rs(LOCAL_KIND::LOCAL_DUMMY, VALTY_UNKNOWN, 0, ins_cnt);
    Is_True_Warn(wasm_stack.size() > 0,
                ("Not enough element on wasm stack when adding mov before branching."));
    if (wasm_stack.size() == 0) {
      return 0;
    }
    if (pop_stack) {
      rs = Pop_wasm_stk(reg_helper, wasm_stack, (VALTYPE) target_stmt.Get_result_type(), ins_cnt);
    } else {
      rs = wasm_stack.top();
    }
    Check_stack_elem_valty(ins_cnt, wasm_stack, rs, target_stmt.Get_result_type());
    // Push a mov then.
    UINTVEC mov_contents;
    mov_contents.push_back(target_stmt.Get_result_reg());
    mov_contents.push_back(rs.Get_idx());
    encoder.Add_inst(MOV,
                     (VALTYPE) target_stmt.Get_result_type(),
                     Get_opc_tyam(MOV), mov_contents);
    return 1;
  }
  return 0;
}

// Add the label of the block's ending, this should happen after the movs are added.
//
void Add_end_of_block_label(WINST_STMT &curstmt, WINST_STMT &parent_stmt, U_ENCODER &encoder, WINST_STMT_GRAPH &graph) {
  UINT32 end_label_id = Add_label(encoder, false, parent_stmt.Get_id(), true);
  Is_True(end_label_id == parent_stmt.Get_end_label(),
          ("Incorrect end label id, original = %d, now = %d",
            graph.Stmt_id(curstmt.Get_parent()).Get_end_label(), end_label_id));

  // Add an end of if at the end of else.
  if (parent_stmt.Get_opcode() == Else) {
    UINT32 end_if_label_id = Add_label(encoder, false, parent_stmt.Get_else_stmt(), true);
    WINST_STMT &if_stmt = graph.Stmt_id(parent_stmt.Get_else_stmt());
    Is_True_Rel(end_if_label_id == if_stmt.Get_end_label(),
                ("Inserting label failed, old label = %d, "
                 "new label = %d, they should be same",
                  end_if_label_id, if_stmt.Get_end_label()));
  }
}


void Add_mov_inst(UINT32 rd, UINT32 rs, VALTYPE valty, U_ENCODER &encoder, WINST_STMT &stmt) {
  UINTVEC mov_contents;
  mov_contents.push_back(rd);
  mov_contents.push_back(rs);
  encoder.Add_inst(MOV,
                   (VALTYPE) valty,
                   Get_opc_tyam(MOV), mov_contents);
}

void Add_jump_inst(UINT32 label_idx, U_OPCODE opc, U_ENCODER &encoder, WINST_STMT &stmt) {
  UINTVEC content;
  content.push_back(label_idx);
  encoder.Add_inst(opc, I32, Get_opc_tyam(opc), content);
}

void Add_jump_inst(UINT32 rd, UINT32 label_idx, U_OPCODE opc, U_ENCODER &encoder, WINST_STMT &stmt) {
  UINTVEC content;
  content.push_back(rd);
  content.push_back(label_idx);
  encoder.Add_inst(opc, I32, Get_opc_tyam(opc), content);
}

void Add_addi_inst(UINT32 rd, UINT32 rs, INT32 imm, U_ENCODER &encoder, WINST_STMT &stmt) {
  UINTVEC content;
  content.push_back(rd);
  content.push_back(rs);
  content.push_back((UINT32) imm);
  encoder.Add_inst(ADD_RRI, I32, Get_opc_tyam(ADD_RRI), content);
}

void Add_rrr_inst(U_OPCODE opc, VALTYPE valty, UINT32 rd, UINT32 rs, UINT32 rt, U_ENCODER &encoder, WINST_STMT &stmt) {
  UINTVEC content;
  content.push_back(rd);
  content.push_back(rs);
  content.push_back(rt);
  encoder.Add_inst(opc, valty, Get_opc_tyam(opc), content);
}

void Add_const_inst(U_OPCODE opc, VALTYPE valty, UINT32 rd, UINT32 imm, U_ENCODER &encoder, WINST_STMT &stmt) {
  UINTVEC content;
  content.push_back(rd);
  content.push_back(imm);
  encoder.Add_inst(opc, valty, Get_opc_tyam(opc), content);
}

void Read_br_table_lbl_vec(UINTVEC &br_labels, CONST_BUFFER pc) {
  // Read the vec(l*: labelidx)
  UINT32 vec_size = Read_leb128(pc); // read the vector's size
  UINT32 vec_size_field_len = Read_opr_size_leb128(pc);
  pc += vec_size_field_len; // move after the size field.
  for(int i = 0; i < vec_size; i++) {
    // Read each labelidx
    br_labels.push_back(Read_leb128(pc));
    UINT32 next_opr_size = Read_opr_size_leb128(pc);
    pc += next_opr_size;
  }
  // Read the final Ln:labelidx
  UINT32 next_opr_size = Read_opr_size_leb128(pc);
  br_labels.push_back(Read_leb128(pc));
  Is_True_Rel(vec_size + 1 == br_labels.size(),
              ("Incomplete reading of brtable's labels, "
               "vec_size+1 = %d, br_labels.size = %d",
                vec_size + 1, br_labels.size()));
}

void Wasm_to_uwasm(U_ENCODER &encoder, WINSVEC &instr,
                   CONST_BUFFER inst_buf, UINT32 len, FILE_CONTEXT &ctx) {
  // Start the process of traversing the function.
  LABEL_TAB              label_tab;
  LOCAL_TAB              local_tab[VALTY_COUNT];
  LOCAL_MAP              wasm_local_ty_map;
  REG_ALLOC_HELPER       reg_helper;
  WASMSTK                wasm_stack;
  vector<UINT32>         call_insts_pc;
  UINT32                 func_id   = Create_ubh_code(ctx.Get_func(), encoder,
                                                     local_tab,
                                                     wasm_local_ty_map,
                                                     reg_helper,
                                                     ctx);
  UINT32                 ins_cnt   = BLOCK_INSTR_BEGIN;
  map<UINT32, UINT32>    wasm_to_uwasm_pc_map; // map pc for debugging
  map<UINT32, UINT32>    icall_type_idx_map;   // map for future usage.
  WINST_STMT_GRAPH       graph;

  // Result in 'graph'
  Build_block_hierarchy(encoder, instr, label_tab, graph);

  // Add the instructions to initialize the locals.
  for (UINT32 i = ctx.Get_func()->GetNumParams(); i < wasm_local_ty_map.size(); i++) {
    LOCAL_MAP_ELEM &map_leme = wasm_local_ty_map.at(i);
    VALTYPE ty = map_leme.Get_ty();
    Is_True(map_leme.Get_map_kind() == LOCAL_KIND::LOCAL,
            ("Not valid local elem in local_map at i = %d", i));
    Is_Trace(Convtrace(TCK_INSTR_CONV),
             (TFile, "Add const 0 for local(reg = %d, ty = %d)\n",
               map_leme.Get_reg_idx(), map_leme.Get_ty()));
    if (ty == I32 || ty == I64) {
      Add_const_inst(U_OPCODE::CONST, ty, map_leme.Get_reg_idx(), 0, encoder, graph.Stmt_id(0));
    } else if (ty == F32) {
      Add_const_inst(U_OPCODE::CONST_L, ty, map_leme.Get_reg_idx(), 0, encoder, graph.Stmt_id(0));
    } else if (ty == F64) {
      Add_const_inst(U_OPCODE::CONST_L, ty, map_leme.Get_reg_idx(), 0, encoder, graph.Stmt_id(0));
      Add_const_inst(U_OPCODE::CONST_L_HIGH, ty, map_leme.Get_reg_idx(), 0, encoder, graph.Stmt_id(0));
    }
  }

  Is_Trace(Convtrace(TCK_MAIN), (TFile, "====== Stack-based pass to translate to uWASM ======\n"));
  for (auto wasm_inst = instr.begin(); wasm_inst != instr.end(); wasm_inst++, ins_cnt++) {
    // Get uwinst from inst map
    // NOTE THAT THE UWINST ALREADY HAS VALTYPE
    // Find the current wasm pc in wasm_pc_to_label_idx map,
    // if there is a label designated here, insert it.
    U_OPC_TY_PAIR  uwasm_pair = UOPCODE_from_wasm(wasm_inst->Encode());
    U_OPC_TY_PAIR *uwasm_inst = &uwasm_pair;
    WINST_STMT    &curstmt    = graph.Stmt_by_inst(ins_cnt);
    TY_AM          ty_am      = uwasm_inst->Get_ty_am();
    wasm_to_uwasm_pc_map.insert(std::make_pair(ins_cnt, encoder.Get_inst_encoder(func_id).Get_inst_size()));
    vector<UINT32> content;
    content.clear();
    UINT32 uwasm_pc = 0;
    switch (wasm_inst->Encode()) {
      case Block: {
        // Allocate a target temp register, but don't push to stack
        //jump_cnt = Read_leb128((BUFFER) wasm_inst->Pc() + 1);
        if (graph.Stmt_by_inst(ins_cnt).Get_result_cnt() > 0) {
          VALTYPE blocktype = (VALTYPE) graph.Stmt_by_inst(ins_cnt).Get_result_type();
          UINT32  reg_id    = Allocate_register(blocktype, ins_cnt, wasm_stack, local_tab, reg_helper);
          graph.Stmt_by_inst(ins_cnt).Set_result_reg(reg_id);
          // Write the results to that register at the 'end' and later use it after the 'end' instruction.
        }
        curstmt.Set_stack_size(wasm_stack.size());
        continue;
      }
      case Loop: {
        UINT32 label_id = Add_label(encoder, true, graph.Get_stmt_id_by_inst(ins_cnt), true);
        Is_True(graph.Stmt_by_inst(ins_cnt).Get_stmt_kind() == WINST_STMT_KIND::WINST_STMT_BLOCK, ("Incorrect graph"));
        Is_True(graph.Stmt_by_inst(ins_cnt).Get_begin_label() == label_id,
                ("Incorrect label used."));
        // Allocate a target temp register, but don't push to stack
        // jump_cnt = Read_leb128((BUFFER) wasm_inst->Pc() + 1);
        if (graph.Stmt_by_inst(ins_cnt).Get_result_cnt() > 0) {
          VALTYPE blocktype = (VALTYPE) curstmt.Get_result_type();
          UINT32  reg_id    = Allocate_register(blocktype, ins_cnt, wasm_stack, local_tab, reg_helper);
          curstmt.Set_result_reg(reg_id);
          // Write the results to that register at the 'end' and later use it after the 'end' instruction.
        }
        curstmt.Set_stack_size(wasm_stack.size());
        continue;
      }
      case Else: {
        // Push a MOV if applicable, for the 'if' block that comes before 'else'.
        Add_mov_for_break(encoder, graph, curstmt.Get_id(), reg_helper, wasm_stack, ins_cnt, false);

        // Push a jump then.
        content.push_back(curstmt.Get_end_label());
        encoder.Add_inst(uwasm_inst->Get_opcode(),
                         uwasm_inst->Get_valty(),
                         uwasm_inst->Get_ty_am(), content);

        // Add a label for if to jump here. (else.begin)
        UINT32 label_id = Add_label(encoder, true, curstmt.Get_id(), true);
        Is_True(curstmt.Get_stmt_kind() == WINST_STMT_KIND::WINST_STMT_BLOCK, ("Incorrect graph"));
        Is_True(curstmt.Get_begin_label() == label_id,
                ("Incorrect label used."));
        content.clear();
        continue;
      }
      case Br: {
        // Unconditional jump
        UINT32 jump_cnt = Read_leb128((BUFFER) wasm_inst->Pc() + 1);
        UINT32 target_label_id = Get_label_id_for_jump(ins_cnt, jump_cnt, graph);
        // TODO: Add a MOV instruction if the block has return-type
        Is_True(target_label_id != UNINIT_LABEL_IDX,
                ("Label id is not valid for br, label = %d, jump_cnt = %d, ins_cnt = %d",
                  target_label_id, jump_cnt, ins_cnt));
        content.push_back(target_label_id);

        // Push a MOV if applicable
        UINT32 target_blk_stmtid = Get_parent_stmt_for_branch(ins_cnt, graph, jump_cnt);
        Add_mov_for_break(encoder, graph, target_blk_stmtid, reg_helper, wasm_stack, ins_cnt, false);
        break;
      }
      case If: {
        WASM_STACK_ELEM rd = Pop_wasm_stk(reg_helper, wasm_stack, I32, ins_cnt);
        if (graph.Stmt_by_inst(ins_cnt).Get_result_cnt() > 0) {
          VALTYPE blocktype = (VALTYPE) curstmt.Get_result_type();
          UINT32  reg_id    = Allocate_register(blocktype, ins_cnt, wasm_stack, local_tab, reg_helper);
          curstmt.Set_result_reg(reg_id);
          if (curstmt.Has_else()) {
            graph.Stmt_id(curstmt.Get_else_stmt()).Set_result_reg(reg_id);
          }
          // Write the results to that register at the 'end' and later use it after the 'end' instruction.
        }
        UINT32 target_label_id = curstmt.Get_end_label();
        if (curstmt.Has_else()) {
          target_label_id = graph.Stmt_id(curstmt.Get_else_stmt()).Get_begin_label();
        }
        Check_stack_elem_valty(ins_cnt, wasm_stack, rd, I32);
        curstmt.Set_stack_size(wasm_stack.size());
        if (curstmt.Has_else()) {
          graph.Stmt_id(curstmt.Get_else_stmt()).Set_stack_size(wasm_stack.size());
        }
        content.push_back((REG_IDX) rd.Get_idx());
        content.push_back(target_label_id);
        break;
      }
      case BrOnExn: {
        Is_True_Rel(false, ("NYI, BrOnExn not implemented."));
      }
      case BrIf:{
        // Expand of stack infos.
        Is_True(!wasm_stack.empty(), ("No value on stack, maybe we got something wrong..."));
        WASM_STACK_ELEM rd = Pop_wasm_stk(reg_helper, wasm_stack, I32, ins_cnt);
        // Conditional jump target calculation.
        UINT32 br_cnt = 0;
        UINTVEC target_labels;
        br_cnt = Read_leb128((BUFFER) wasm_inst->Pc() + 1);
        target_labels.push_back(br_cnt);
        UINT32 target_label_id = Get_label_id_for_jump(ins_cnt, br_cnt, graph);
        // UINT32 target_label_id = Add_label(encoder, label_tab.Get(target.Get_label_id()).Get_pc(), false);
        content.push_back((REG_IDX) rd.Get_idx());
        content.push_back(target_label_id);
        UINT32 total_items_popped = 0;
        // Push MOVs before the jump if applicable
        for (UINTVEC::iterator it = target_labels.begin(); it != target_labels.end(); it++) {
          UINT32 target_blk_stmtid = Get_parent_stmt_for_branch(ins_cnt, graph, *it);
          // don't pop any item from stack now.
          total_items_popped += Add_mov_for_break(encoder, graph, target_blk_stmtid, reg_helper, wasm_stack, ins_cnt, false);
        }
//        // Pop one item from stack
//        if (total_items_popped > 0) {
//          Pop_wasm_stk(reg_helper, wasm_stack);
//        }
        break;
      }
      case BrTable: {
        // Allocate a temp register
        UINT32 rtemp_idx = Push_temp_reg(ins_cnt, I32, wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        WASM_STACK_ELEM rtemp = Pop_wasm_stk(reg_helper, wasm_stack, I32, ins_cnt);
        // Expand of stack infos.
        Is_True(!wasm_stack.empty(), ("No value on stack, maybe we got something wrong..."));
        WASM_STACK_ELEM rd = Pop_wasm_stk(reg_helper, wasm_stack, I32, ins_cnt);
        // Conditional jump target calculation.
        UINTVEC target_labels;
        UINTVEC br_labels;
        CONST_BUFFER pc = (CONST_BUFFER) wasm_inst->Pc() + 1;
        Read_br_table_lbl_vec(br_labels, pc);
        Is_True_Rel(br_labels.size() > 0,
                    ("Cannot process Brtable with branch-label vector size == 0, "
                     "ins_cnt = %d, size = %lu", ins_cnt, br_labels.size()));
        Is_True_Warn(br_labels.size() > 1,
                     ("There may be inefficiency during process Brtable with branch-label vector size >= 1, "
                      "ins_cnt = %d, label_size = %lu\n", ins_cnt, br_labels.size()));

        // Add mov rd to temp register
        Add_mov_inst(rtemp.Get_idx(), rd.Get_idx(), I32, encoder, curstmt);

        // Deduplicate br_labels.
        UINTSET temp_dedup(br_labels.begin(), br_labels.end());
        target_labels.assign(temp_dedup.begin(), temp_dedup.end());
        INT32 br_labels_vec_sz = br_labels.size();
        UINT32 total_items_popped = 0;
        // Push MOVs before the jump if applicable first.
        for (UINTVEC::iterator it = target_labels.begin(); it != target_labels.end(); it++) {
          UINT32 target_blk_stmtid = Get_parent_stmt_for_branch(ins_cnt, graph, *it);
          // don't pop any item from stack now.
          total_items_popped += Add_mov_for_break(encoder, graph, target_blk_stmtid, reg_helper, wasm_stack, ins_cnt, false);
        }
        // Add more jumps
        for (INT32 i = 0; i < br_labels_vec_sz; i++) {
          UINT32 label_br_cnt = br_labels.at(i);
          UINT32 target_label_id = Get_label_id_for_jump(ins_cnt, label_br_cnt, graph);
          // UINT32 target_label_id = Add_label(encoder, label_tab.Get(target.Get_label_id()).Get_pc(), false);
          if (i == br_labels_vec_sz - 1) { // the last one should be unconditional jump
            Add_jump_inst(target_label_id, U_OPCODE::JMP, encoder, curstmt);
          } else {
            Add_jump_inst(rtemp.Get_idx(), target_label_id, U_OPCODE::JEZ, encoder, curstmt);
          }
          if (i < br_labels_vec_sz - 1) { // the last two item should not have 'add' instr.
            Add_addi_inst(rtemp.Get_idx(), rtemp.Get_idx(), -1, encoder, curstmt);
          }
        }
        continue;
      }
      case Drop: {
        Pop_wasm_stk(reg_helper, wasm_stack, VALTY_UNKNOWN, ins_cnt);
        continue;
      }
      case GlobalGet: {
        BUFFER pc = (BUFFER)wasm_inst->Pc() + 1;
        UINT64 global_idx = (UINT64) Read_leb128(pc);
        UINT32 uwasm_glbl_id = ctx.Get_global_uwasm_id(global_idx);
        // Get the valty from wasm's global table
        wabt::Type wasmty = ctx.Get_module()->GetGlobal(wabt::Var(global_idx))->type;
        VALTYPE valty = Conv_wabt_ty(wasmty);
        UINT32 rd = Push_temp_reg(ins_cnt,valty, wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        uwasm_inst->Set_valty(valty);
        content.push_back((REG_IDX) rd);
        content.push_back(uwasm_glbl_id);
        break;
      }
      case GlobalSet: {
        BUFFER pc = (BUFFER)wasm_inst->Pc() + 1;
        UINT64 global_idx = (UINT64) Read_leb128(pc);
        UINT32 uwasm_glbl_id = ctx.Get_global_uwasm_id(global_idx);
        // Get the valty from wasm's global table
        wabt::Type wasmty = ctx.Get_module()->GetGlobal(wabt::Var(global_idx))->type;
        VALTYPE valty = Conv_wabt_ty(wasmty);
        WASM_STACK_ELEM rs = Pop_wasm_stk(reg_helper, wasm_stack, valty, ins_cnt);
        uwasm_inst->Set_valty(valty);
        content.push_back((REG_IDX) rs.Get_idx());
        content.push_back(uwasm_glbl_id);
        break;
      }
      case Unreachable: {
        Push_unreachable_value(wasm_stack, ins_cnt);
        continue;
      }
      case Select: {
        /**
         * Resulting sequence is like:
         *         MOV Rd,     Rs(value when bool = 0)
         *         JEZ Rbool,  LabelA
         *         MOV Rd,     Rt(value when bool = 1)
         * LabelA:
         */
        // Allocate a temp register to store select_bool
        UINT32 rtemp_idx = Push_temp_reg(ins_cnt, I32, wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        WASM_STACK_ELEM rtemp = Pop_wasm_stk(reg_helper, wasm_stack, I32, ins_cnt);
        Is_True_Rel(wasm_stack.size() >= 3,
                    ("There must be at least 4 items on stack for "
                     "'select' at %d, yet we only have %lu",
                      ins_cnt, wasm_stack.size()));
        WASM_STACK_ELEM select_bool = Pop_wasm_stk(reg_helper, wasm_stack, I32, ins_cnt);
        WASM_STACK_ELEM rs = Pop_wasm_stk(reg_helper, wasm_stack, VALTY_UNKNOWN, ins_cnt);
        VALTYPE valty = rs.Get_val_type();
        WASM_STACK_ELEM rt = Pop_wasm_stk(reg_helper, wasm_stack, valty, ins_cnt);
        UINT32 rd = Push_temp_reg(ins_cnt,valty, wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);

        // Prepare the label
        char label_name[28];
        sprintf(label_name, ".L_select_%u", ins_cnt);
        UINT32 label_idx = encoder.Request_label(label_name);

        if (rd == rt.Get_idx()) {
          // Add a jump
          Add_jump_inst(select_bool.Get_idx(), label_idx, U_OPCODE::JNZ, encoder, curstmt);
          // Add a mov
          Add_mov_inst(rd, rs.Get_idx(), valty, encoder, curstmt);
        } else if (select_bool.Get_idx() == rd) {
          // Add MOV select bool to rtemp, need to preserve select_bool since it's going to be overriden.
          Add_mov_inst(rtemp.Get_idx(), select_bool.Get_idx(), I32, encoder, curstmt);
          // Add MOV
          Add_mov_inst(rd, rs.Get_idx(), valty, encoder, curstmt);
          // Add a jump
          Add_jump_inst(rtemp.Get_idx(), label_idx, U_OPCODE::JEZ, encoder, curstmt);
          // Add a mov
          Add_mov_inst(rd, rt.Get_idx(), valty, encoder, curstmt);
        } else {
          // Add MOV
          Add_mov_inst(rd, rs.Get_idx(), valty, encoder, curstmt);
          // Add a jump
          Add_jump_inst(select_bool.Get_idx(), label_idx, U_OPCODE::JEZ, encoder, curstmt);
          // Add a mov
          Add_mov_inst(rd, rt.Get_idx(), valty, encoder, curstmt);
        }

        // Add a lable for landing.
        UINT32 result_label_idx = encoder.Add_label(label_name);
        Is_True_Rel(result_label_idx == label_idx,
                    ("Label must match, yet %d and %d",
                      label_idx, result_label_idx));
        content.clear();
        continue;
      }
      case Nop:
      {
        // TODO
        continue;
      }
      case LocalGet: {
        BUFFER pc = (BUFFER)wasm_inst->Pc() + 1;
        UINT32 local_idx         = Read_leb128(pc);
        LOCAL_MAP_ELEM &rs       = Get_wasm_local_ty(wasm_local_ty_map, (LOCAL_IDX) local_idx);
        LOCAL_TAB_ELEM &tab_elem = local_tab[rs.Get_ty()].Get(Get_tab_idx_by_reg_idx(rs.Get_reg_idx()));
        WASM_STACK_ELEM r = WASM_STACK_ELEM(tab_elem.Kind(), rs.Get_ty(), rs.Get_reg_idx(), ins_cnt);
        wasm_stack.push(r);
        continue;
      }
      case LocalSet:
      case LocalTee: {
        BUFFER pc = (BUFFER)wasm_inst->Pc() + 1;
        UINT32 local_idx = Read_leb128(pc);
        LOCAL_MAP_ELEM &rd = Get_wasm_local_ty(wasm_local_ty_map, (LOCAL_IDX) local_idx);
        WASM_STACK_ELEM rs(LOCAL_KIND::UNREACHABLE_VALUE, VALTY_UNKNOWN, 0, ins_cnt);
        if (wasm_inst->Encode() == LocalSet) {
          rs = Pop_wasm_stk(reg_helper, wasm_stack, rd.Get_ty(), ins_cnt);
        } else if (wasm_inst->Encode() == LocalTee) {
          rs = wasm_stack.top();
          Check_stack_elem_valty(ins_cnt, wasm_stack, rs, rd.Get_ty());
        } else {
          Is_True_Rel(false,
                  ("Unexpected opcode met = %d", wasm_inst->Encode()));
        }
        uwasm_inst->Set_valty(rs.Get_val_type());
        content.push_back((REG_IDX) rd.Get_reg_idx());
        content.push_back((REG_IDX) rs.Get_idx());
        break;
      }
      case I32Load8S:
      case I32Load8U:
      case I32Load16S:
      case I32Load16U:
      case I64Load8S:
      case I64Load8U:
      case I64Load16S:
      case I64Load16U:
      case I64Load32S:
      case I64Load32U:
      case I32Load:
      case I64Load:
      case F32Load:
      case F64Load: {
        /**
         * Three resulting forms: (offst = offset)
         * 1. offst <= 8bit
         * load_s Rd,   Rs,  offst
         *
         * 2. offst > 8bit, offst <= 22bit
         * load_l Rd,    Rs,  offst
         *
         * 3. offst > 22 bit, offst < 32bit.
         * const   Rtemp  offst
         * add     Rtemp  Rs
         * load_s  Rd,    Rtemp,  0
         */
        BUFFER pc = (BUFFER)wasm_inst->Pc() + 1 + Read_opr_size_leb128((BUFFER) wasm_inst->Pc() + 1);
        // Allocate temp register
        UINT32 rtemp_idx = Push_temp_reg(ins_cnt, I32, wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        WASM_STACK_ELEM rtemp = Pop_wasm_stk(reg_helper, wasm_stack, I32, ins_cnt);
        WASM_STACK_ELEM rs = Pop_wasm_stk(reg_helper, wasm_stack, I32, ins_cnt);
        UINT32 rd = Push_temp_reg(ins_cnt, uwasm_inst->Get_valty(), (UINT64)pc, wasm_stack, local_tab, reg_helper);
        Is_True_Rel((UINT64) Read_leb128(pc) <= OFFSET_L_MAX, ("Number exceed maximum of 32bit value."));
        content.push_back(rd);
        content.push_back(rs.Get_idx());
        UINT64 offset = (UINT64) Read_leb128(pc);
        content.push_back(offset);
        if (offset > OFFSET_S_MAX || rs.Get_idx() > REG_S_MAX || rd > REG_S_MAX) {
          Is_True_Rel(offset <= OFFSET_L_MAX, ("offset out of bound = %llu(%llx). ", offset, offset));
          uwasm_inst->Set_opcode(Conv_to_long_opcode(uwasm_inst->Get_opcode()));
          if (offset >= OFFSET_M_MAX) {
            // Use const + add to prepare the full-width i32 value.
            if (rs.Get_idx() < REG_S_MAX && rd < REG_S_MAX && rtemp_idx < REG_S_MAX) {
              // TODO: Use small format.
            }
            Add_const_inst(U_OPCODE::CONST_L, I32, rtemp_idx, offset, encoder, curstmt);
            Add_rrr_inst(U_OPCODE::ADD, I32, rtemp_idx, rs.Get_idx(), rtemp_idx, encoder, curstmt);
            content.clear();
            content.push_back(rd);
            content.push_back(rtemp_idx);
            content.push_back(0);
          }
        }
        break;
      }
      case I32Store8:
      case I32Store16:
      case I64Store8:
      case I64Store16:
      case I64Store32:
      case I32Store:
      case I64Store:
      case F32Store:
      case F64Store: {
        BUFFER pc = (BUFFER)wasm_inst->Pc() + 1 + Read_opr_size_leb128((BUFFER) wasm_inst->Pc() + 1);
        // Pre-allocate a temp-reg idx.
        UINT32 rtemp_idx = Push_temp_reg(ins_cnt, I32, wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        WASM_STACK_ELEM rtemp = Pop_wasm_stk(reg_helper, wasm_stack, I32, ins_cnt);
        WASM_STACK_ELEM rd = Pop_wasm_stk(reg_helper, wasm_stack, uwasm_inst->Get_valty(), ins_cnt);
        WASM_STACK_ELEM rs = Pop_wasm_stk(reg_helper, wasm_stack, I32, ins_cnt);
        Is_True_Rel((UINT64) Read_leb128(pc) < 0x7FFFFFFFu, ("Number exceed maximum of 32bit value."));
        content.push_back(rd.Get_idx());
        content.push_back(rs.Get_idx());
        UINT64 offset = (UINT64) Read_leb128(pc);
        content.push_back(offset);
        if (offset > OFFSET_S_MAX || rs.Get_idx() > REG_S_MAX || rd.Get_idx() > REG_S_MAX) {
          Is_True_Rel(offset <= OFFSET_L_MAX, ("offset out of bound = %llu(%llx). ", offset, offset));
          uwasm_inst->Set_opcode(Conv_to_long_opcode(uwasm_inst->Get_opcode()));
          if (offset >= OFFSET_M_MAX) {
            // Use const + add to prepare the full-width i32 value.
            if (rs.Get_idx() < REG_S_MAX && rd.Get_idx() < REG_S_MAX && rtemp_idx < REG_S_MAX) {
              // TODO: Use small format.
            }
            Add_const_inst(U_OPCODE::CONST_L, I32, rtemp_idx, offset, encoder, curstmt);
            Add_rrr_inst(U_OPCODE::ADD, I32, rtemp_idx, rs.Get_idx(), rtemp_idx, encoder, curstmt);
            content.clear();
            content.push_back(rd.Get_idx());
            content.push_back(rtemp_idx);
            content.push_back(0);
          }
        }
        break;
      }
      case I64Const:
      case F64Const: {
        BUFFER pc = (BUFFER) wasm_inst->Pc() + 1;
        UINT32 rd = Push_temp_reg(ins_cnt,uwasm_inst->Get_valty(), wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        UINT64 const_value = Read_const_value(pc, uwasm_inst->Get_valty());

        // Add a low 32 bit const.
        vector<UINT32> low_content;
        low_content.push_back(rd);
        low_content.push_back(const_value & 0xFFFFFFFFl);
        encoder.Add_inst(U_OPCODE::CONST_L, uwasm_inst->Get_valty(), Get_opc_tyam(U_OPCODE::CONST_L), low_content);

        // Add a high 32 bit const instruction
        content.push_back(rd);
        content.push_back(const_value >> 32u);
        break;
      }
      case I32Const:
      case F32Const: {
        BUFFER pc = (BUFFER) wasm_inst->Pc() + 1;
        UINT32 rd = Push_temp_reg(ins_cnt, uwasm_inst->Get_valty(), wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        content.push_back(rd);
        UINT64 const_value = Read_const_value(pc, uwasm_inst->Get_valty());
        content.push_back(const_value & 0xFFFFFFFFl);
        break;
      }
      case MemoryGrow: {
        WASM_STACK_ELEM rs = Pop_wasm_stk(reg_helper, wasm_stack, I32, ins_cnt);
        UINT32 rd = Push_temp_reg(ins_cnt, uwasm_inst->Get_valty(), wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        content.push_back(rd);
        content.push_back(rs.Get_idx());
        break;
      }
      case MemorySize: {
        UINT32 rd = Push_temp_reg(ins_cnt, uwasm_inst->Get_valty(), wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        content.push_back(rd);
        break;
      }
      // Comparators(testop in the spec), T, T => I32 on stack operation.
      case I32LtS:
      case I32LtU:
      case I64LtS:
      case I64LtU:
      case F32Lt:
      case F64Lt:
      case I32LeS:
      case I32LeU:
      case I64LeS:
      case I64LeU:
      case F32Le:
      case F64Le:
      case I32GtS:
      case I32GtU:
      case I64GtS:
      case I64GtU:
      case F32Gt:
      case F64Gt:
      case I32GeS:
      case I32GeU:
      case I64GeS:
      case I64GeU:
      case F32Ge:
      case F64Ge:
      case I32Ne:
      case I64Ne:
      case F32Ne:
      case F64Ne:
      case I32Eq:
      case I64Eq:
      case F32Eq:
      case F64Eq:
      {
        // 2-to-1 (two pop, one push)
        WASM_STACK_ELEM rt = Pop_wasm_stk(reg_helper, wasm_stack, VALTY_UNKNOWN, ins_cnt);
        WASM_STACK_ELEM rs = Pop_wasm_stk(reg_helper, wasm_stack, rt.Get_val_type(), ins_cnt);
        REG_IDX rd = Push_temp_reg(ins_cnt, I32, wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        content.push_back(rd);
        content.push_back(rs.Get_idx());
        content.push_back(rt.Get_idx());
        break;
      }
      // Binary Mathematical Operators.
      case I32Add:
      case I64Add:
      case F32Add:
      case F64Add:
      case I32Sub:
      case I64Sub:
      case F32Sub:
      case F64Sub:
      case I32Mul:
      case I64Mul:
      case F32Mul:
      case F64Mul:
      case I32DivS:
      case I64DivS:
      case F32Div:
      case F64Div:
      case I32DivU:
      case I64DivU:
      case I32And:
      case I64And:
      case I32Xor:
      case I64Xor:
      case I32Or:
      case I64Or:
      case I32Shl:
      case I64Shl:
      case I32ShrU:
      case I64ShrU:
      case I32ShrS:
      case I64ShrS:
      case I32Rotl:
      case I32Rotr:
      case I64Rotl:
      case I64Rotr:
      case I32RemU:
      case I64RemU:
      case I32RemS:
      case I64RemS:
      {
        // 2-to-1 (two pop, one push)
        WASM_STACK_ELEM rt = Pop_wasm_stk(reg_helper, wasm_stack, VALTY_UNKNOWN, ins_cnt);
        WASM_STACK_ELEM rs = Pop_wasm_stk(reg_helper, wasm_stack, rt.Get_val_type(), ins_cnt);
        // By the spec, the two types should be the same.
        Check_stack_elem_valty(ins_cnt, wasm_stack, rs, rt);
        REG_IDX rd = Push_temp_reg(ins_cnt,uwasm_inst->Get_valty(), wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        content.push_back(rd);
        content.push_back(rs.Get_idx());
        content.push_back(rt.Get_idx());
        break;
      }
      case F32Copysign:
      case F64Copysign:
      {
        // 2-to-1 (two pop, one push, with BUILTIN support)
        WASM_STACK_ELEM rt = Pop_wasm_stk(reg_helper, wasm_stack, uwasm_inst->Get_valty(), ins_cnt);
        WASM_STACK_ELEM rs = Pop_wasm_stk(reg_helper, wasm_stack, uwasm_inst->Get_valty(), ins_cnt);
        REG_IDX rd = Push_temp_reg(ins_cnt,uwasm_inst->Get_valty(), wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        content.push_back(rd);
        content.push_back(rs.Get_idx());
        UINT64 final_opr = (rt.Get_idx() << 16) + (UINT32) Conv_wasm_opc_to_builtin(wasm_inst->Encode());
        content.push_back(final_opr);
        break;
      }
      case F32Neg:
      case F32Ceil:
      case F32Floor:
      case F32Sqrt:
      case F64Neg:
      case F64Ceil:
      case F64Floor:
      case F64Sqrt:
      {
        // RS-to-RD (one stack pop, one stack push)
        WASM_STACK_ELEM rs = Pop_wasm_stk(reg_helper, wasm_stack, uwasm_inst->Get_valty(), ins_cnt);
        REG_IDX rd = Push_temp_reg(ins_cnt,uwasm_inst->Get_valty(), wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        content.push_back(rd);
        content.push_back(rs.Get_idx());
        break;
      }
      case I32Clz:
      case I32Ctz:
      case I32Popcnt:
      case I64Clz:
      case I64Ctz:
      case I64Popcnt:
      case I32WrapI64:
      case I32TruncF32S:
      case I32TruncF32U:
      case I32TruncF64S:
      case I32TruncF64U:
      case I64ExtendI32S:
      case I64ExtendI32U:
      case I64TruncF32S:
      case I64TruncF32U:
      case I64TruncF64S:
      case I64TruncF64U:
      case F32ConvertI32S:
      case F32ConvertI32U:
      case F32ConvertI64S:
      case F32ConvertI64U:
      case F32DemoteF64:
      case F64ConvertI32S:
      case F64ConvertI32U:
      case F64ConvertI64S:
      case F64ConvertI64U:
      case F64PromoteF32:
      case I32ReinterpretF32:
      case I64ReinterpretF64:
      case F32ReinterpretI32:
      case F64ReinterpretI64:
      case F32Trunc:
      case F32Nearest:
      case F32Abs:
      case F64Trunc:
      case F64Nearest:
      case F64Abs:{
        // Math related builtin asms here.
        // RS-to-RD (one stack pop, one stack push)
        WASM_STACK_ELEM rs = Pop_wasm_stk(reg_helper, wasm_stack, VALTY_UNKNOWN, ins_cnt);
        REG_IDX rd = Push_temp_reg(ins_cnt,uwasm_inst->Get_valty(), wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        content.push_back(rd);
        content.push_back(rs.Get_idx());
        content.push_back(Conv_wasm_opc_to_builtin(wasm_inst->Encode())); // IMM Mode, use the original wasm opcode for now.
        break;
      }
      case I64Eqz:
      case I32Eqz: {
        // Rs-to-RD (this is not builtin !!!)
        WASM_STACK_ELEM rs = Pop_wasm_stk(reg_helper, wasm_stack, uwasm_inst->Get_valty(), ins_cnt);
        UINT32 rd = Push_temp_reg(ins_cnt, I32, wasm_inst->Pc(), wasm_stack, local_tab, reg_helper);
        content.push_back(rd);
        content.push_back(rs.Get_idx());
        break;
      }
      case CallIndirect:
      case Call: {
        BUFFER pc = (BUFFER) wasm_inst->Pc() + 1;
        wabt::FuncSignature *callee = nullptr;
        FUNC_IDX callee_idx_or_type_idx;
        if(wasm_inst->Encode() == Call) {
          callee_idx_or_type_idx = Read_leb128(pc); // read function idx from wasm inst, set as funcidx
          callee = &ctx.Get_module()->GetFunc(wabt::Var(callee_idx_or_type_idx))->decl.sig;
        } else if (wasm_inst->Encode() == CallIndirect) {
          FUNC_IDX callee_type_idx = Read_leb128(pc); // read function idx from wasm inst
          callee = &ctx.Get_module()->GetFuncType(wabt::Var(callee_type_idx))->sig;
          callee_idx_or_type_idx = callee_type_idx;  // Set to type idx
        } else {
          Is_True(false, ("incorrect wasm_inst opcode given = %d", wasm_inst->Encode()));
        }
        UINT32      param_sz  = callee->GetNumParams();
        UINT32 i; UINT32 call_param_num[VALTY_COUNT] = {0};

        WASM_STACK_ELEM callee_idx_elem(LOCAL_KIND::TEMP, I64, 0, ins_cnt);
        if (wasm_inst->Encode() == CallIndirect) {
          callee_idx_elem = Pop_wasm_stk(reg_helper, wasm_stack, I32, ins_cnt);
        }

        // before CALL, we need to prepare formal params for callee
        // this include several MOV insts
        // for i in [1, callee_param_num]:
        //   mov wasm_stack.top to reg[local_num + RET_REG_NUM + i]
        //   wasm_stack.pop()
        // note that we dont know local_num now, so we need to fixup Rds later
        Is_Trace_cmd(Convtrace(TCK_CALL_CONV), { Print_func_sig(callee); });
        vector<WASM_STACK_ELEM> param_values;
        for (i = 0; i < param_sz; i++) {
          WASM_STACK_ELEM rs = Pop_wasm_stk(reg_helper, wasm_stack, VALTY_UNKNOWN, ins_cnt);
          param_values.push_back(rs);
          VALTYPE expected = Conv_wabt_ty(callee->GetParamType(param_sz - 1 - i));
          Is_True_Rel(expected == rs.Get_val_type(),
                      ("The param %d in call at %d has a invalid type, "
                       "expected = %d, real = %d, func_id = %d",
                        i, ins_cnt,expected, rs.Get_val_type(),
                        callee_idx_or_type_idx,
                        (Print_stack(param_values), Print_stack(wasm_stack), 1)));
        }
        // Reverse the param list due for making sure it aligns with callee's signature.
        std::reverse(param_values.begin(), param_values.end());
        // Print the param value list.
        Is_Trace_cmd(Convtrace(TCK_CALL_CONV), { Print_stack(param_values, TFile); });
        // Fix 'mov's' Rd.
        for (i = 0; i < param_sz; i++) {
          WASM_STACK_ELEM &rs = param_values.at(i);
          U_OPC_TY_PAIR  mov_inst   = UOPCODE_inst(MOV, rs.Get_val_type());
          TY_AM          cur_ty_am  = mov_inst.Get_ty_am();
          call_param_num[rs.Get_val_type()]++; // call func param num
          // mov reg[?], wasm_stack.top
          vector<UINT32> mov_content;
          mov_content.clear();
          mov_content.push_back(0);
          mov_content.push_back(rs.Get_idx());
          encoder.Add_inst(mov_inst.Get_opcode(), mov_inst.Get_valty(), cur_ty_am, mov_content);

          // Extra check to make sure the status is correct.
          Is_True(Conv_wabt_ty(callee->GetParamType(i))
                  == rs.Get_val_type(),
                  ("Reversal of param_values failed, "
                   " i = %d, at inst = %d", i, ins_cnt));
        }

        // set max call param needed(for vm to allocate space)
        for (i = VALTY_MIN; i < VALTY_COUNT; ++i) {
          if (local_tab[i].Get_max_call_param() < call_param_num[i])
            local_tab[i].Set_max_call_param(call_param_num[i]);
        }

        UINT32 call_pc = encoder.Get_inst_encoder(func_id).Get_inst_size();
        call_insts_pc.push_back(call_pc); // store CALL inst pc
        wasm_to_uwasm_pc_map.insert(std::make_pair(ins_cnt, call_pc));

        if (wasm_inst->Encode() == Call) {
          // push CALL inst
          content.push_back(Conv_func_idx(callee_idx_or_type_idx));
          encoder.Add_inst(uwasm_inst->Get_opcode(), uwasm_inst->Get_valty(), uwasm_inst->Get_ty_am(), content);
        } else {
          Is_True(wasm_inst->Encode() == CallIndirect, ("%s, wasm_inst should be CallIndirect, yet it is %d", NYI, wasm_inst->Encode()));
          // push CALL_INDIRECT inst
          Is_True(callee_idx_elem.Get_val_type() == I32,
                  ("CALL_INDIRECT's callee index should be I32 == 0 ?, "
                   "yet it is %d, wasm.callpc = %d", callee_idx_elem.Get_val_type(), ins_cnt));
          content.push_back(callee_idx_elem.Get_idx());
          Conv_type_idx(callee_idx_or_type_idx, encoder, ctx.Get_module());
          icall_type_idx_map.insert(std::make_pair(call_pc, callee_idx_or_type_idx));
          encoder.Add_inst(uwasm_inst->Get_opcode(), uwasm_inst->Get_valty(), uwasm_inst->Get_ty_am(), content);
        }

        VALTYPE callee_ret_valty;
        UINT32  callee_ret_tmp_idx;
        UINT32  callee_ret_num = callee->GetNumResults();
        // after CALL, we need to MOV callee return value to caller's reg space,
        // because callee ret regs may be used by other functions
        // for i in (1, RET_REG_NUM]:
        //   push reg[local_num + i] into wasm_stack(MOV temp_reg, ret_reg)
        // note that we dont know local_num now, so we need to fixup Rses later
        Is_True(callee_ret_num < 2, ("function return more than 1, callee func idx = %d", callee_idx_or_type_idx));
        for (i = 0; i < callee_ret_num; i++) {
          callee_ret_tmp_idx = Push_temp_reg(ins_cnt,Conv_wabt_ty(callee->GetResultType(i)),
                                             (UINT64) pc, wasm_stack, local_tab,
                                             reg_helper);
          callee_ret_valty   = Conv_wabt_ty(callee->GetResultType(wabt::Index(i)));
          U_OPC_TY_PAIR mov_inst = UOPCODE_inst(MOV, callee_ret_valty);
          vector<UINT32> mov_content;
          mov_content.clear();
          mov_content.push_back(callee_ret_tmp_idx);
          mov_content.push_back(0);
          encoder.Add_inst(mov_inst.Get_opcode(),
                           mov_inst.Get_valty(),
                           mov_inst.Get_ty_am(),
                           mov_content);
        }
        continue;
      }
      case Return: {
        UINT32 res_num = ctx.Get_func()->GetNumResults();
        Is_True(res_num < 3, ("function return more than 2, funcidx=%d", ctx.Get_func_idx()))
        // FIXME: THe quote here is dubious
        //  'WASMSTK temp_stack(wasm_stack); // not like end, return wont pop '
        // mov stack.top to ret reg
        for (UINT32 i = 0; i < res_num; i++) {
          WASM_STACK_ELEM rs = Pop_wasm_stk(reg_helper, wasm_stack,
                                            (VALTYPE) Conv_wabt_ty(ctx.Get_func()->GetResultType(i)),
                                            ins_cnt);
          U_OPC_TY_PAIR mov_inst = UOPCODE_inst(MOV, rs.Get_val_type());
          vector<UINT32> mov_content;
          mov_content.clear();
          mov_content.push_back((REG_IDX)(i + CONST_REG_NUM));
          mov_content.push_back((REG_IDX)rs.Get_idx());
          encoder.Add_inst(mov_inst.Get_opcode(),
                           mov_inst.Get_valty(),
                           mov_inst.Get_ty_am(),
                           mov_content);
        }
        break;
      }
      case End: {
        WINST_STMT &parent_stmt = graph.Stmt_id(curstmt.Get_parent());
        // Add Label;
        if (graph.Stmt_id(curstmt.Get_parent()).Get_stmt_kind()
            != WINST_STMT_KIND::WINST_STMT_ROOT_BLOCK) {
          // Remove the unreachable value from TOS if the value seems to be unreachable.
          if (wasm_stack.size() > 0 && wasm_stack.top().Get_kind() == LOCAL_KIND::UNREACHABLE_VALUE) {
            wasm_stack.pop();
            Pop_redundant_values(parent_stmt, reg_helper, wasm_stack);
            Push_result_to_stack(parent_stmt, wasm_stack);
            Add_end_of_block_label(curstmt, parent_stmt, encoder, graph);
            break;
          } else if (parent_stmt.Get_result_cnt() > 0 &&
              (parent_stmt.Get_opcode() == Block ||
               parent_stmt.Get_opcode() == Else  ||
               parent_stmt.Get_opcode() == If)) {
            // There should be no "unreachable" thus far.
            WASM_STACK_ELEM rs(LOCAL_KIND::LOCAL_DUMMY, I32, 0, 0);
            // In case of a If-else block.
            if (parent_stmt.Get_opcode() == If && parent_stmt.Has_else()) {
               rs = wasm_stack.top();
            } else {
               rs = Pop_wasm_stk(reg_helper, wasm_stack, (VALTYPE) parent_stmt.Get_result_type(), ins_cnt);
            }
            Is_True_Rel(rs.Get_val_type() == parent_stmt.Get_result_type(),
                        ("Incorrect stack item valty = %d, blocktype expected = %d",
                          rs.Get_val_type(), parent_stmt.Get_result_type()));
            Is_True(parent_stmt.Get_result_reg() != 0,
                    ("The result register should not be zero at inst = %d", ins_cnt));
            // Add mov of result to the reserved register.
            VALTYPE mov_valty = (VALTYPE) parent_stmt.Get_result_type();
            // Push a jump then.
            UINTVEC mov_contents;
            mov_contents.push_back(parent_stmt.Get_result_reg());
            mov_contents.push_back(rs.Get_idx());
            encoder.Add_inst(U_OPCODE::MOV,
                             mov_valty,
                             Get_opc_tyam(U_OPCODE::MOV), mov_contents);
          }
          Add_end_of_block_label(curstmt, parent_stmt, encoder, graph);
          // other conditions include Loop, and If/Block/Else with 0/more results.
          // any non-function block here as parent_stmt.
          Pop_redundant_values(parent_stmt, reg_helper, wasm_stack);
          Push_result_to_stack(parent_stmt, wasm_stack);
          continue;
        } // if (!ROOT_BLOCK)
        // End of function, ROOT_BLOCK here as parent_stmt
        UINT32 res_num = ctx.Get_func()->GetNumResults();
        UINT32 stack_size = wasm_stack.size();
        Is_True(res_num < 3,
                ("function returns more than 2 results, funcidx=%d",
                  ctx.Get_func_idx()));
        if (res_num <= 0) {
          // nothing to do, normal function end.
          // Add a return instr.
          break;
        } else if (stack_size > 0) { // res_num > 0 && stack_size > 0
          BOOL found_unreachable = false;

          // Investigate each item in wasm_stack.
          vector<WASM_STACK_ELEM> temp_stack;
          for (UINT32 i = 0; i < stack_size; i++) {
            WASM_STACK_ELEM &result = wasm_stack.top();
            if (result.Get_kind() == LOCAL_KIND::UNREACHABLE_VALUE) {
              // There is 'unreachable' before, 'any value' should apply here.
              found_unreachable = true;
              break;
            }
            temp_stack.push_back(result);
            wasm_stack.pop();
          }

          // There is 'unreachable prior the end', ignore moving the results.
          if (found_unreachable) {
            continue;
          }
          // Reorganize the wasm_stack.
          for (UINT32 i = 0; i < stack_size; i++) {
            wasm_stack.push(temp_stack.back());
            temp_stack.pop_back();
          }
          if (wasm_stack.size() == res_num) {
            // mov stack.top to r0, r1
            for (UINT32 i = 0; i < res_num; i++) {
              // TODO: Use the real function's types instead of VALTY_UNKNOWN
              WASM_STACK_ELEM rs       = Pop_wasm_stk(reg_helper, wasm_stack, VALTY_UNKNOWN, ins_cnt); // end pop
              U_OPC_TY_PAIR   mov_inst = UOPCODE_inst(MOV, rs.Get_val_type());
              vector<UINT32>  mov_content;
              mov_content.clear();
              mov_content.push_back((REG_IDX) (i + CONST_REG_NUM));
              mov_content.push_back((REG_IDX) rs.Get_idx());
              encoder.Add_inst(mov_inst.Get_opcode(),
                               mov_inst.Get_valty(),
                               mov_inst.Get_ty_am(),
                               mov_content);
            }
            break;
          }
        }


        if (graph.Stmt_by_inst(ins_cnt - 1).Get_opcode() == Return) {
          // Special treatment
          Is_Trace(Convtrace(TCK_STACK_CHK),
                   (TFile, "End of function after return, no MOV "
                    "on end of function, stack size = %lu, should be %d\n",
                    wasm_stack.size(), res_num));
          // No need to add an extra return-instr.
          continue;
        }
        Is_Trace(Convtrace(TCK_STACK_CHK),
                 (TFile, "Incorrect wasm code met, insufficient item found, no MOV "
                  "on end of function, stack size = %lu, should be %d\n",
                  wasm_stack.size(), res_num));
        Is_True_Rel(!ctx.Is_stack_check(),
                    ("Incorrect wasm code met, insufficient item found, no MOV "
                     "on end of function, stack size = %lu, should be %d",
                     wasm_stack.size(), res_num));
        // Add a return instr.
        break;
      }
      default:
        Is_True(false, ("unknown wasm op, (%d) 0x%02x", wasm_inst->Encode(), wasm_inst->Encode()));
        continue;
    }
    encoder.Add_inst(uwasm_inst->Get_opcode(),
                     uwasm_inst->Get_valty(),
                     uwasm_inst->Get_ty_am(),
                     content);
  }

  /**
   * REG stack:
   * ==========================
   * | REG IDX |    REG TYPE  |
   * |    0    |      RET     |
   * |    1    |      RET     |
   * |   ...   |     LOCAL    |
   * | local+1 |  CALLEE RET  |  <- we don't know local number until now
   * | local+2 |  CALLEE RET  |
   * |   ...   | CALLEE PARAM |
   * ==========================
   * */

  // fix up MOV insts around CALL
  for (UINT32 & pc : call_insts_pc) {
    U_CODE_BODY &code_block = encoder.Get_inst_encoder(func_id);
    UINT32 &original_instr = code_block.Inst_tab().at(pc);
    U_INST32 local_instr32 = original_instr;
    Is_True(local_instr32.Opcode() == CALL || local_instr32.Opcode() == CALL_INDIRECT,
            ("expect a call at %d, get %s", pc,
              UWINST_NAME[local_instr32.Opcode()]));

    // Get local_total_num.
    UINT32 local_total_num[4] = {0};
    for (UINT32 valtype = 0; valtype < VALTY_COUNT; valtype++) {
      local_total_num[valtype] = RET_REG_NUM + local_tab[valtype].size();
      Is_Trace(Convtrace(TCK_CALL_CONV),
               (TFile, "=== local_size[%u] = %u\n", valtype, local_tab[valtype].size()));
      Is_Trace(Convtrace(TCK_CALL_CONV),
               (TFile, "=== max_callee[%u] = %u\n", valtype, local_tab[valtype].Get_max_call_param()));
      Is_Trace(Convtrace(TCK_CALL_CONV),
               (TFile, "=== local_total_num[%u] = %u\n", valtype, local_total_num[valtype]));
    }

    // change rd in MOVes before CALL
    wabt::FuncSignature *sig = nullptr;
    if (local_instr32.Opcode() == CALL) {
      FUNC_IDX func_idx = local_instr32.OFS._ofs; // this is a func_idx.
      wabt::Func *callee     = ctx.Get_module()->GetFunc(wabt::Var(func_idx));
      sig = &callee->decl.sig;
    } else if (local_instr32.Opcode() == CALL_INDIRECT) {
      FUNC_IDX type_idx = icall_type_idx_map.at(pc); // the is a type_idx.
      wabt::FuncType *callee     = ctx.Get_module()->GetFuncType(wabt::Var(type_idx));
      sig = &callee->sig;
    } else {
      Is_True(false,
              ("Incorrect opcode = %d, %s",
               local_instr32.Opcode(),
               UWINST_NAME[local_instr32.Opcode()]))
    }
    UINT32 param_sz        = sig->GetNumParams();
    UINT32 callee_ret_num  = sig->GetNumResults();
    UINT32 param_used_in_valty[VALTY_COUNT] = {0};
    for (UINT32 i = 0; i < param_sz; i++) {
      UINT32 mov_pc = pc - param_sz + i;
      // Which of the instruction in the 64 bit, the lower 32 or the higher 32bit.
      // const UINT32 is_mov_l32 = mov_pc % 2;
      UINT32   &mov_instr    = code_block.Inst_tab().at(mov_pc);
      U_INST32 loc_mov_instr = mov_instr;
      VALTYPE  valtype       = loc_mov_instr.Valtype();
      Is_True_Rel(loc_mov_instr.Opcode() == MOV,
              ("expect a mov at %d, get %s", mov_pc, UWINST_NAME[loc_mov_instr.Opcode()]));
      Is_True_Rel(loc_mov_instr.Ty_am() == TY_AM32_R_R,
              ("expect mov to be using TY_AM32_R_R at %d, but we have %d", mov_pc, loc_mov_instr.Ty_am()));

      UINT32 mov_target = local_total_num[valtype] + RET_REG_NUM + param_used_in_valty[valtype];
      // Override the instruction content in the table
      ((U_INST32 *) &(code_block.Inst_tab().at(mov_pc)))->R_R._r1 = mov_target;
      param_used_in_valty[valtype]++;
    }

    // change rs in the MOV after CALL
    Is_True(callee_ret_num <= 1,
            ("The callee should only return 1 value. Not %u", callee_ret_num));
    if (callee_ret_num == 1) {
      for (UINT32 param_idx = 0; param_idx < callee_ret_num; param_idx++) {
        UINT32 mov_pc = (pc + 1) + param_idx; // first mov = pc + 1
        UINT32   &mov_instr    = code_block.Inst_tab().at(mov_pc);
        U_INST32 loc_mov_instr = mov_instr;
        VALTYPE  valtype       = loc_mov_instr.Valtype();
        Is_True(loc_mov_instr.Opcode() == MOV,
                ("expect a mov at %d, get %s", mov_pc, UWINST_NAME[loc_mov_instr.Opcode()]));
        Is_True(loc_mov_instr.Ty_am() == TY_AM32_R_R,
                ("expect mov to be using TY_AM32_R_R at %d, but we have %d", mov_pc, loc_mov_instr.Ty_am()));

        UINT32 mov_target =  local_total_num[valtype] + param_idx;
        // Override the instruction content in the table
        ((U_INST32 *) &(code_block.Inst_tab().at(mov_pc)))->R_R._r2 = mov_target;
      }
    } // otherwise, there would be zero return value, hence nothing to be done.
  }

  // Recalculate label table
  // FIXME: probably wrong here
  // for (UINT32 i = 0; i < label_tab.Size() ; i++)
  //   label_tab.Get((LABEL_IDX) i).Set_pc(wasm_to_uwasm_pc_map[label_tab.Get((LABEL_IDX) i).Get_pc()]);

  UINT32 locals_size[4], io_size[4];
  for (UINT32 i = 0; i < VALTY_COUNT; i++) {
    // Recalibrate the local size and io_reg size
    locals_size[i] = local_tab[i].size() + RET_REG_NUM;
    // The callees and callers' ret-regs.
    io_size[i] = local_tab[i].Get_max_call_param() + RET_REG_NUM * 2;
  }
  encoder.Get_inst_encoder(func_id).Update_ubh_code(locals_size, io_size, 0);

  // if (Convtrace(TCK_FUNC_FINAL))
  // if (Convtrace(TCK_FUNC_FINAL)) u_code.Print_local_table(outfile);
  // if (Convtrace(TCK_FUNC_FINAL)) local_tab.Print();
  Is_Trace(Convtrace(TCK_FUNC_FINAL), (TFile, "====== label tabel ======\n"));
  if (Convtrace(TCK_FUNC_FINAL)) label_tab.Print(TFile);
  if (Convtrace(TCK_FUNC_WASM_MAP)) {
    Is_Trace(Convtrace(TCK_FUNC_FINAL), (TFile, "====== wasm to uwasm mapping ======\n"));
    Print_wasm_map(wasm_to_uwasm_pc_map, encoder, func_id, instr, TFile);
  } else if (Convtrace(TCK_FUNC_FINAL)) {
    encoder.Get_inst_encoder(func_id).Print(TFile);
  }
  Is_Trace(Convtrace(TCK_MAIN), (TFile, "====== translate complete ======\n"));
}


void WINST_STMT::Init() {
  _begin_label = UNINIT_LABEL_IDX;
  _end_label   = UNINIT_LABEL_IDX;
}
