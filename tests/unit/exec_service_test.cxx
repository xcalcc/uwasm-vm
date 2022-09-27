//
// Created by lishijie on 9/10/20.
//
// need to arrange as unit test here
#if 0
#include "exec_service.h"

int main(int argc, char **argv) {
  UWASM_SERVIDE &service = Service();
  // Allocate new register frame
  service.Machine().Reg_i32().Alloc(100);
  service.Machine().Reg_i64().Alloc(100);
  service.Machine().Reg_f32().Alloc(100);
  service.Machine().Reg_f64().Alloc(100);

  Is_Trace(Tracing(TP_EXEC),
           (TFile, "Entering demo func, writing result to file\n"));

  BIN_FILE_ACCESS file;
  UBH_TYPE ubh_type(0xDEADBEEF);
  vector< pair<VALTYPE, UINT32> > locals;
  locals.push_back(std::make_pair(I32, 3));
  locals.push_back(std::make_pair(I64, 2));
  locals.push_back(std::make_pair(F32, 3));
  locals.push_back(std::make_pair(F64, 2));
  UINT32 ty_idx = file.Get_encoder().Add_type(ubh_type);
  UINT32 func_idx = file.Get_encoder().Add_func((char *) "test_func_1", ty_idx, &locals, (char *) "exported");
  file.Get_encoder().Insert_label("test_func_1", "l1");
  file.Get_encoder().Append_inst("test_func_1", CONST, I32, (REG_IDX) 0x02, (MASTIFF_LONG) 0x01);
  file.Get_encoder().Append_inst("test_func_1", CONST, I32, (REG_IDX) 0x03, (MASTIFF_LONG) 0x02);
  file.Get_encoder().Append_inst("test_func_1", ADDS, I32, (REG_IDX) 0x04, (REG_IDX) 0x03, (REG_IDX) 0x02);
  file.Get_encoder().Append_inst("test_func_1", MOV, I32, (REG_IDX) 0x00, (REG_IDX) 0x04);
  file.Get_encoder().Append_inst("test_func_1", RETURN, I32);

  // file.Get_encoder().Add_global("global_test_1", false, );

  file.Dump_as_file("test.out"); // auto closed.
  Load_from_file("test.out");
}
#endif