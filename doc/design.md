# Goals

## What kind of feature do we want for the VM?
- Speed  
- Security  
- Isolation
- Not a general purpose VM, merely for connecting JS and binary code.

## What do we want from the whole system.
- Productivity
  - Script Language (such as Python, JS, MiniApp)
    - 兼容
  - Instrumented/Automatic acceleration ?
  - Ecosystem
- Performance
  - Compiler, VM, Runtime.
- Security
  - Isolation (gVisor, Sandbox)
   - Resolves AOB, NPD, MSF
  - Address protection
   - Emscripten's integrity check.
   - Control flow integrity check, CFG integrity, Global Table integrity
- CPU Agnostic
   - Lots of chip vendors


## System, VM design diagram
- Need refinedment
- Need to fit the usage of matrix multiply (as CNN)
- App <--> VM(Driver, as in CUDA) <--> Algorithms (in binary form)

## What are the concerns:
 
- IPAOT
   - Bring xvsa inside runtime.

## Concerns
- Concerns
  - Dynamic
   - Lambda, track stack, (compile time lambda)
     - Whether this is a template / runtime feature.
   - Closure
   - Matrix Multiply
     - The multiplication
   - Why not directly bind with V8?
     - Perhaps the function call
   - Foreign function interface overhead
   - WASM <-> uWASM <-> JS co-exist?
   - Access management
   - Sandbox model.
   - Perhaps a runtime needed ?
   - Consistency,
     - As to say the handling of global, local aggregates, stacks, heap
     - Data copyless transfer between UVM and v8
   - Instruction Set
     - CISC or RISC ?
     - DSA/DSP special builtin instructions.
     - Floating point arithmetics
   - Binary format / decoding
     - Maybe bytecode could be acceptable
     - Binary size inflation against with WASM
     - Variable length instruction
     - CG decided instruction format/length ?
   - Platform support for general purpose apps.
     - XNN and matrix computation centric
     - Other arithmetics and `fastm` like
     - What are the major apps running on the platform
   - Vectorizing / GPU / NPU ?
     - A platform independent representation of a vector
     - Two-tier code optimization / generation
     - CPU Agnosticism
   - Parallelization
     - Computation model
     - Resources model (ownerships and relationships)
     - Sharing resources between instances
     - Compatible with V8 / Other host platforms?
   - Security
     - Emscripten   - WASM's existing concerns in terms of security.
     - DoS, RCE, Sensitive information leakage
     - Isolation / Escaping Prevention
     - Permission / Access control / libc api level permission

## TODO and Unfinished Items
- Topics
   - Scanning with XVSA and code review
   - Restructure the project with clarified concepts etc.,
   - Local Aggregates
   - Debugging info and debugging utilities
     - Mini-DWARF or DWARF
     - stripped and non-stripped version
   - Module level linking
   - WASM to uWASM, compatibility issues
   - Memory optimisation
   - Tail Call Optimization ?

# Authors

### For different parts, without particular order

```
Guanting Lu   jason.lu
Zhou Fang     ark.fang
Shijie Li     shijie.li
Long Li       long.li
Qing Zhu      qing.zhu
Jianxin Lai   jianxin.lai
```

### Under supervision from
```
Sun Chan      sun.chan
```