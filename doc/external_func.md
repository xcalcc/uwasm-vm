# External References:
WASI API Doc:
https://github.com/WebAssembly/WASI/blob/master/phases/snapshot/docs.md

# Design Goal
1. Efficient and Fast Native call
2. Extension friendly way of handling native calls.
3. Simpler way of adding a new function or include many new functions.
4. Multiple library / module friendly (not a priority now, but must be extensible enough to allow future integration)


# Proposal A
### Setting up the libraries
1. Using generally available compilers to compile the library (LIB) source code, and dump the LIST of function-names exported in the resulting shared object.
2.  Copy the function names list (LISTs) + (LIBs) shared object to the uvm execution directory.

### Building the uWASM binary file
1. Using xvsa to compile the source code into the uWASM files. 
2. During compilation, mark all the unseen function as EXTERNAL and add them into an import table
3. Each external function has a in-module function number, namely local-func-#.


### Runtime fixup & execution
1. Use the function name list and shared object to build a map of function name to its code in the shared object (using dlsym), namely, the global functions table, and build up a global type table.
2. For each uWASM module loaded
   1. Build an exported function table and add to the global functions table.
   2. Iterate over the import table and locate the entry in the gloabl functions table by matching the function names, 
   3. Fill up the structures in the local function table in that module with global-func-#.
      1. This sturcture must contain at least a 'global function id', 'function definition module id' etc., 
   4. Continue building up the module caching tables.

3. Verification of the function prototypes during the fixup.

### Calling external func in a uWASM module
1. when an external function call occured, the local-func-# is known.
2. the the entry in the module-level functions table is picked up, in order to read its global-func-#
3. The entry from the global function table is picked up and its handler address / function prototype known
4. Use the function prototype to determine its parameters, and pass that param to the handler(using call to the address).
5. Return value gets saved to the return-register. 

# Proposal B
## Difference form Proposal A
- 1. Do the local to global mapping at decode time or at run-time.
- 2. Let the compiler be aware of the native-function-ids of the these 'native' calls and let compiler generate these IDs inside the uWASM binary file.


# Concerns
1. Furture compatibility
2. API Definitions
3. API-level security control
4. Library version number, integrity check, compatibility check ...
5. Utility functions like 
   1. Is_uwasm_func_avail(const char *feature name);
   2. Get_uwasm_native_func(const char *feature name);




