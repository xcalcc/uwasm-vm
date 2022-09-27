/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef UWASM_VM_U_TRACE_H
#define UWASM_VM_U_TRACE_H

#include "u_types.h"

#define AssertDev(Condition, Params) \
        (Condition ? (void) 1 \
        :(Quit_with_tracing ( __FILE__, __LINE__, __func__),	\
        Assertion_Failure_Print Params) );

#define WarnDev(Condition, Params) \
        (Condition ? (void) 1 \
        :(Quit_with_tracing ( __FILE__, __LINE__, __func__),	\
        Warning_Failure_Print Params) );

#define ErrorUnless(Condition, Params) \
        (Condition ? (void) 1 \
        :(Quit_with_tracing ( __FILE__, __LINE__, __func__),	\
        Assertion_Failure_Print Params) );


#define TFile stderr
#define SBAR "-----------------------------------------------------------------------\n"
#define DBAR "=======================================================================\n"
#define UIMPL "NYI"
#define NYI "NYI"
#define TRACE_ENABLE_LEVEL 1
#define TRACE_UNABLE_LEVEL 0

#ifdef UWASM_VM_RELEASE
#define Is_Trace(cond, msg) ((void) 1)
#define If_Trace(cond, statements) ((void) 1)
#define Is_Trace_cmd(Cond, Cmd)  ((void) 1)
#define Is_Trace_cmd_rel(Cond, Cmd) { if (Cond) Cmd ; }
#define Is_True(cond, msg)
#define Is_True_Rel ((void) 1);
#define Is_True_Warn WarnDev
#else
#define Is_Trace(cond, msg) { if ((cond)) { fprintf msg; } }
#define If_Trace(cond, statements) { if (cond) statements; }
#define Is_Trace_cmd(Cond, Cmd) { if (Cond) Cmd ; }
#define Is_Trace_cmd_rel(Cond, Cmd) { if (Cond) Cmd ; }
#define Is_True AssertDev
#define Is_True_Rel ErrorUnless
#define Is_True_Warn WarnDev
#endif


// Add a performance special cmd option,
// which is by-default disabled at release mode.
#if defined(UWASM_VM_RELEASE) && !defined(UWASM_VM_RELEASE_PERF)
#define Is_Perf_cmd(Cond, Cmd) ((void) 1);
#else
#define Is_Perf_cmd(Cond, Cmd) { if (Cond) Cmd ; }
#endif

// This is vulnerable to changes
typedef enum {
  TP_BIN_READ  = 1,
  TP_BIN_WRITE = 2,
  TP_EXEC      = 3,
  TP_EPERF     = 4,
  TP_CONV      = 5,
  TP_SLD       = 6,
  TP_DEBUG     = 7,
  TRACE_KIND_MAX = 10, // 0x40
} TRACE_KIND;

UINT32 Set_tracing_opt(TRACE_KIND kind, UINT32 level);
#ifdef UWASM_VM_RELEASE_NO_PROFILE
#define Tracing(kind) ((bool) 0)
#else
bool Tracing(TRACE_KIND);
#endif
void Quit_with_tracing(const char *, UINT32, const char *); // Quiting
void Assertion_Failure_Print ( const char *fmt, ... ); // Printf-like function
void Warning_Failure_Print ( const char *fmt, ... ); // Printf-like function

#endif //UWASM_VM_U_TRACE_H
