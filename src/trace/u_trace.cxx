/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include "u_trace.h"

class TRACE_OPT_INFO {
public:
  UINT32                            _global_lvl;
  UINT32                           _kind_to_level[TRACE_KIND_MAX];
  UINT32 Get(TRACE_KIND tk) {
    return _kind_to_level[tk];
  }
  void Set(TRACE_KIND tk, UINT32 level) {
    Is_True(tk < TRACE_KIND_MAX, ("Invalid trace kind ... %d", tk));
    _kind_to_level[(UINT32) tk] = level;
  }
  UINT32 Level() {
    return _global_lvl;
  }
  TRACE_OPT_INFO() {
    // Initialize defaults.
    _global_lvl = TRACE_ENABLE_LEVEL;
    for (UINT32 i = 0; i < TRACE_KIND_MAX; i++) {
      _kind_to_level[i] = 0;
    }
  }
};

TRACE_OPT_INFO trace_info;

void Assertion_Failure_Print ( const char *fmt, ... )
{
  va_list vp;

  char msg[512];
  INT32 pos;

  /* Prepare header line: */
  va_start ( vp, fmt );
  pos = sprintf ( &msg[0], "### Assertion Failure : \n");
  pos += vsprintf ( &msg[pos], fmt, vp );
  sprintf ( &msg[pos], "\n" );
  va_end ( vp );
  fprintf(stderr, "%s\n", msg);
  exit(1);
}


void Warning_Failure_Print ( const char *fmt, ... )
{
  va_list vp;

  char msg[512];
  INT32 pos;

  /* Prepare header line: */
  va_start ( vp, fmt );
  pos = sprintf ( &msg[0], "### Warning : \n");
  pos += vsprintf ( &msg[pos], fmt, vp );
  sprintf ( &msg[pos], "\n" );
  va_end ( vp );
  fprintf(stderr, "%s\n", msg);
}

void Comp_Failure_Print ( const char *fmt, ... )
{
  va_list vp;

  char msg[512];
  INT32 pos;

  /* Prepare header line: */
  va_start ( vp, fmt );
  pos = sprintf ( &msg[0], "Compilation failure: \n");
  pos += vsprintf ( &msg[pos], fmt, vp );
  sprintf ( &msg[pos], "\n" );
  va_end ( vp );
  fprintf(stderr, "%s\n", msg);
  exit(2);
}

/***
 *  Quit the processing with trace
 **/
void Quit_with_tracing(const char * file, UINT32 line, const char *func_name) {
  fprintf(stderr, "\n\n\n### In file %s:%d, func: %s\n", file, line, func_name);
}

#ifndef UWASM_VM_RELEASE_NO_PROFILE
bool Tracing(TRACE_KIND tc) {
  return trace_info.Get(tc) >= trace_info.Level();
}
#endif

UINT32 Set_tracing_opt(TRACE_KIND kind, UINT32 level) {
  trace_info.Set(kind, level);
  return level;
}
