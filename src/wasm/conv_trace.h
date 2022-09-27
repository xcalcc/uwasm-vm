//
// Created by xc5 on 1/26/21.
//

#ifndef UWASM_VM_CONV_TRACE_H
#define UWASM_VM_CONV_TRACE_H

enum TRACE_CONV_KIND {
    TCK_ANY = 0,
    TCK_DUMMY = 1,
    TCK_BLOCK = 2,
    TCK_WASM_READER = 3,
    TCK_LABEL = 4,
    TCK_MAIN = 5,
    TCK_UBH = 6,
    TCK_CALL_CONV = 7,
    TCK_FUNC_FINAL = 8,
    TCK_INSTR_CONV = 9,
    TCK_STMTS_GRAPH = 10,
    TCK_STACK_CHK = 11,
    TCK_FUNC_WASM_MAP = 12,
    TCK_LAST,
    TCK_COUNT = TCK_LAST + 1
};

class CONV_TRACE_OPT_INFO {
public:
    UINT32                            _global_lvl;
    UINT32                           _kind_to_level[TRACE_CONV_KIND::TCK_COUNT];
    UINT32 Get(TRACE_CONV_KIND tk) {
      return _kind_to_level[tk];
    }
    void Set(TRACE_CONV_KIND tk, UINT32 level) {
      Is_True(tk < TRACE_CONV_KIND::TCK_COUNT, ("Invalid trace kind ... %d", tk));
      _kind_to_level[(UINT32) tk] = level;
    }
    UINT32 Level() {
      return _global_lvl;
    }
    CONV_TRACE_OPT_INFO() {
      // Initialize defaults.
      _global_lvl = TRACE_ENABLE_LEVEL;
      for (UINT32 i = 0; i < TRACE_CONV_KIND::TCK_COUNT; i++) {
        _kind_to_level[i] = 0;
      }
    }
};

// Tracing utility function for Is_Trace and Is_Trace_cmd
bool Convtrace(TRACE_CONV_KIND conv_kind);
void Set_conv_trace(TRACE_CONV_KIND conv_kind, UINT32 level);

#endif //UWASM_VM_CONV_TRACE_H
