uWASM ISA Definition

## Registers

Each register is labelled with a 10-biit index.
Thus having a limit of totally at most 1024 registers.

We will use R0 - R1023 to represent them.

#### Dedicated registers

There are 2 registers that is used for ABI purpose. 
- R0 = return register 1
- R1 = return register 2

#### Other registers' use


We are now considering to put the R2-R1023 into five instruction classes.
- FORMAL: The parameter given by the caller
- LOCAL:  The locals
- TEMP:   The temporaries
- CALLEE_RET:    The callee's return values. (Shared with callee)
- CALLEE_PARAM:  Params to pass to the callee. (Shared with callee)

## Data Types

There are four data types holded in registers and used in instructions currently, they are:
- i32
- i64
- u32
- u64 

## Instructions

The instructions are either 32-bit long or 64-bit long.
The 0-bit (little-endian) in the first byte determines whether this should be a 64-bit instruction or 32-bit instruction.  

We have the following kind of instruction patterns

- Adress-mode-32, which contains a 7-bit opcode
- Adress-mode-32_R, which contains a 7-bit opcode, and a 10-bit register number
- Adress-mode-32_OFS, which contains a 7-bit opcode, and a 22-bit offset
- Adress-mode-32_R_OFS, which contains a 7-bit opcode, and a 10-bit register number, and 10-bit offset/immediate number
- Adress-mode-32_R_R, which contains a 7-bit opcode, and a 10-bit register number, and 10-bit register number
- Adress-mode-64_R_R_R, which contains a 7-bit opcode, and three 10-bit register numbers.
- Adress-mode-64_R_OFS, which contains a 7-bit opcode, and a 10-bit register number, and a 32-bit offset/immediate number 
- Adress-mode-64_R_R_OFS, which contains a 7-bit opcode, and two 10-bit register numbers, and a 24-bit offset/immediate number

We devide the instructions into following groups:

### Numeric Instructions

Numeric(Arithmetic instructions are often based on registers)
They are mostly: 
- AM32_R_R
- AM64_R_R_R
- AM32_R
- AM64_R_OFL

Examples:

- i32.add AM64_R_R_R
- i32.mul AM64_R_R_R
- i32.eqz AM32_R_R
- i32.ne  AM64_R_R_R
- i32.const AM64_R_OFL
- i64.const AM64_R_OFL
- f32.const AM64_R_OFL
- f64.const AM64_R_OFL


### Memory instructions

There are three major kind of memory instructions

#### Load and stores

This include
- load: AM64_R_R_OFS. ``load[x bit]_[s/u]`` means load x bit Singed/Unsigned from memory, e.g.
    - ``load8``: load 8 bit from memory
    - ``load8_s``: load 8 bit signed from memory
    - if [x bit] is absent, will load bits according to the valtype, e.g. ``i64.load`` loads 64 bit, while ``i32.load`` loads 32 bit
    - floats have only two load insts ``f32.load`` and ``f64.load``
    - example: ``i32.load r4, r5, 0x123456`` means ``r4 = mem[0x123456 + r5]``
- store: AM64_R_R_OFS. Same with load, (only i32 and i64) store has ``store[x bit]``(but no signed/unsigned).
    - example: ``i32.store r4, r5, 0x123456`` means ``mem[0x123456 + r5] = r4``

#### Memory utilities
- memory.size   AM32_R
- memory.grow   AM32_R


#### Control Instructions

Labels are stored in a table comes with the binary.

Controls are determined by labels, jumps, conditional jumps, and function-calls.

- jeq, jne, jlt, jle, jgt, jge    AM64_R_R_OFS
    - example: ``jeq r4, r5, 0x123456`` means ``jmp 0x123456 if r4 == r5``
- jez, jnz, jlz, jlez, jgz, jgez  AM32_R_OFS, 10-bit offset
    - example: ``jlz r4, 0x123456`` means ``jmp 0x123456 if r4 < 0``

- call AM32_OFS,  this has a function index of 10-bit.
- indirect.call AM32_R,  this uses a function index stored in a 12-bit indexed register


## VM Execution

### Label Table
Each function has a label table, relative to the function starting pc, e.g.

```asm
foo:
0x1234567 add r9, r5, r6
......
jmp 0x2; jmp to func_end
......
func_end:
0x1234578 return
```

The label table of foo is:

| label index | relative offset |
| ---- | ---- |
| 1 | 0x5 |
| 2 | 0xb |

Then ``jmp 0x2`` means set pc to ``0x1234567 + 0xb = 0x12345678``

### Calling

The call instruction should be like
```asm
call $call_ofst
```

Where the local table should contains something like the following:

|  Register Index | Get_kind | Content |
|  ----  | ----  | ---- |
|  2  | Formal Param | (set by VM)  |
|  ... | Formal Param | (set by VM)  |
|  i > 3 | Local/Temp | (local or temporaries usable by code)  |
|  i + 1 | Local/Temp | (local or temporaries usable by code)  |
|  ...  | Local/Temp | (local or temporaries usable by code)  |
|  j  | Param area | (will be used as actual param in calls) |
|  j + 1  | Param area | (will be used as actual param in calls) |
|  ...  | Param area | (will be used as actual param in calls) |

Calling of each function is described in the following section.


### Internal actions done during calling

1. Finding the function pointer under the offset
2. Reserve the a block for locals for the callee function in the register stack
3. Store the register base pointer (RBP) somewhere (probably on register stack)
4. Update the register base pointer (RBP)
5. Copy the register param to the callee local block
6. Execute the next function
7. When that function returns, reset the register base pointer (RBP)
8. Continuing executing the caller function  


### Dealing with recursions

If the VM follows the above workings, the recursion should be resolved just fine.
The function execution of a call graph
```
call graph:
A -> B---> C
     |---> A  (recursion)
     \---> D 
```

The stack before A calling B

|  Function in stack |
| ---- |
|  A |

The stack before B calling C

|  Function in stack |
| ---- |
|  A |
|  B |

The stack while C is executing

|  Function in stack |
| ---- |
|  A |
|  B |
|  C |


The stack before B calling A

|  Function in stack |
| ---- |
|  A |
|  B |

The stack after B called A

|  Function in stack |
| ---- |
|  A |
|  B |
|  A |


The stack after A returns to B

|  Function in stack |
| ---- |
|  A |
|  B |

The stack after B called D

|  Function in stack |
| ---- |
|  A |
|  B |
|  D |


## Appendix

a current instruction table. 

|name|                   uwasm_op|                 ty_am|           valtype|
| ------ | ------ | ------ | ------ |
| control inst |
|"nop"|                  NOP|                     TY_AM32|         Void|
|"unreach"|              UNREACH|                 TY_AM32|         Void|
|"block"|                BLOCK|                   TY_AM32_R|       Void|
|"try"|                  TRY|                     TY_AM32_R|       Void|
|"catch"|                CATCH|                   TY_AM32_R|       Void|
|"throw"|                THROW|                   TY_AM32_R|       Void|
|"return"|               RETURN|                  TY_AM32|         Void|
|"call"|                 CALL|              TY_AM32_OFS|     Void|
|"call_indirect"|        CALL_INDIRECT|           TY_AM32_R|       Void|
|"return_call"|          RETURN_CALL|             TY_AM64_R_OFS|   Void|
|"return_call_indirect"| RETURN_CALL_INDIRECT|    TY_AM64_R_OFS|   Void|
|"select"|               SELECT|                  TY_AM64_R_R_R|   Void|
|"jmp"|                  JMP|                     TY_AM32_OFS|     Void|
|"jeq"|                  JEQ|                     TY_AM64_R_OFS|   Void|
|"jne"|                  JNE|                     TY_AM64_R_OFS|   Void|
|"jlt"|                  JLT|                     TY_AM64_R_OFS|   Void|
|"jle"|                  JLE|                     TY_AM64_R_OFS|   Void|
|"jgt"|                  JGT|                     TY_AM64_R_OFS|   Void|
|"jge"|                  JGE|                     TY_AM64_R_OFS|   Void|
|"jez"|                  JEZ|                     TY_AM32_R_OFS|   Void|
|"jnz"|                  JNZ|                     TY_AM32_R_OFS|   Void|
|"jlz"|                  JLZ|                     TY_AM32_R_OFS|   Void|
|"jlez"|                 JLEZ|                    TY_AM32_R_OFS|   Void|
|"jgz"|                  JGZ|                     TY_AM32_R_OFS|   Void|
|"jgez"|                 JGEZ|                    TY_AM32_R_OFS|   Void|
| memory inst |
|"i32.load"|             LOAD|                    TY_AM64_R_R_OFS| I32|
|"i64.load"|             LOAD|                    TY_AM64_R_R_OFS| I64|
|"f32.load"|             LOAD|                    TY_AM64_R_R_OFS| F32|
|"f64.load"|             LOAD|                    TY_AM64_R_R_OFS| F64|
|"i32.load8_s"|          LOAD8_S|                 TY_AM64_R_R_OFS| I32|
|"i32.load8_u"|          LOAD8_U|                 TY_AM64_R_R_OFS| I32|
|"i32.load16_s"|         LOAD16_S|                TY_AM64_R_R_OFS| I32|
|"i32.load16_u"|         LOAD16_U|                TY_AM64_R_R_OFS| I32|
|"i64.load8_s"|          LOAD8_S|                 TY_AM64_R_R_OFS| I64|
|"i64.load8_u"|          LOAD8_U|                 TY_AM64_R_R_OFS| I64|
|"i64.load16_s"|         LOAD16_S|                TY_AM64_R_R_OFS| I64|
|"i64.load16_u"|         LOAD16_U|                TY_AM64_R_R_OFS| I64|
|"i64.load32_s"|         LOAD32_S|                TY_AM64_R_R_OFS| I64|
|"i64.load32_u"|         LOAD32_U|                TY_AM64_R_R_OFS| I64|
|"i32.store"|            STORE|                   TY_AM64_R_R_OFS| I32|
|"i64.store"|            STORE|                   TY_AM64_R_R_OFS| I64|
|"f32.store"|            STORE|                   TY_AM64_R_R_OFS| F32|
|"f64.store"|            STORE|                   TY_AM64_R_R_OFS| F64|
|"i32.store8"|           STORE8|                  TY_AM64_R_R_OFS| I32|
|"i32.store16"|          STORE16|                 TY_AM64_R_R_OFS| I32|
|"i64.store8"|           STORE8|                  TY_AM64_R_R_OFS| I64|
|"i64.store16"|          STORE16|                 TY_AM64_R_R_OFS| I64|
|"i64.store32"|          STORE32|                 TY_AM64_R_R_OFS| I64|
|"memory.size"|          MEMORYSIZE|              TY_AM32_R|       Void|
|"memory.grow"|          MEMORYGROW|              TY_AM32_R|       Void|
| numeric inst |
|"i32.const"|            CONST|                   TY_AM64_R_OFS|   I32|
|"i64.const"|            CONST|                   TY_AM64_R_OFS|   I64|
|"f32.const"|            CONST|                   TY_AM64_R_OFS|   F32|
|"f64.const"|            CONST|                   TY_AM64_R_OFS|   F64|
|"i32.eqz"|              EQZ|                     TY_AM64_R_OFS|   I32|
|"i32.eq"|               EQ|                      TY_AM64_R_R_R|   I32|
|"i32.ne"|               NE|                      TY_AM64_R_R_R|   I32|
|"i32.lt_s"|             LT_S|                    TY_AM64_R_R_R|   I32|
|"i32.lt_u"|             LT_U|                    TY_AM64_R_OFS|   I32|
|"i32.gt_s"|             GT_S|                    TY_AM64_R_OFS|   I32|
|"i32.gt_u"|             GT_U|                    TY_AM64_R_OFS|   I32|
|"i32.le_s"|             LE_S|                    TY_AM64_R_OFS|   I32|
|"i32.le_u"|             LE_U|                    TY_AM64_R_OFS|   I32|
|"i32.ge_s"|             GE_S|                    TY_AM64_R_OFS|   I32|
|"i32.ge_u"|             GE_U|                    TY_AM64_R_OFS|   I32|
|"i64.eqz"|              EQZ|                     TY_AM64_R_OFS|   I64|
|"i64.eq"|               EQ|                      TY_AM64_R_OFS|   I64|
|"i64.ne"|               NE|                      TY_AM64_R_OFS|   I64|
|"i64.lt_s"|             LT_S|                    TY_AM64_R_OFS|   I64|
|"i64.lt_u"|             LT_U|                    TY_AM64_R_OFS|   I64|
|"i64.gt_s"|             GT_S|                    TY_AM64_R_OFS|   I64|
|"i64.gt_u"|             GT_U|                    TY_AM64_R_OFS|   I64|
|"i64.le_s"|             LE_S|                    TY_AM64_R_OFS|   I64|
|"i64.le_u"|             LE_U|                    TY_AM64_R_OFS|   I64|
|"i64.ge_s"|             GE_S|                    TY_AM64_R_OFS|   I64|
|"i64.ge_u"|             GE_U|                    TY_AM64_R_OFS|   I64|
|"f32.eq"|               EQ|                      TY_AM64_R_OFS|   F32|
|"f32.ne"|               NE|                      TY_AM64_R_OFS|   F32|
|"f32.lt"|               LT_S|                    TY_AM64_R_OFS|   F32|
|"f32.gt"|               GT_S|                    TY_AM64_R_OFS|   F32|
|"f32.le"|               LE_S|                    TY_AM64_R_OFS|   F32|
|"f32.ge"|               GE_S|                    TY_AM64_R_OFS|   F32|
|"f64.eq"|               EQ|                      TY_AM64_R_OFS|   F64|
|"f64.ne"|               NE|                      TY_AM64_R_OFS|   F64|
|"f64.lt"|               LT_S|                    TY_AM64_R_OFS|   F64|
|"f64.gt"|               GT_S|                    TY_AM64_R_OFS|   F64|
|"f64.le"|               LE_S|                    TY_AM64_R_OFS|   F64|
|"f64.ge"|               GE_S|                    TY_AM64_R_OFS|   F64|
|"i32.clz"|              CLZ|                     TY_AM64_R_OFS|   I32|
|"i32.ctz"|              CTZ|                     TY_AM64_R_OFS|   I32|
|"i32.popcnt"|           POPCNT|                  TY_AM64_R_OFS|   I32|
|"i32.add"|              ADD|                     TY_AM64_R_R_R|   I32|
|"i32.sub"|              SUB|                     TY_AM64_R_R_R|   I32|
|"i32.mul"|              MUL|                     TY_AM64_R_R_R|   I32|
|"i32.div_s"|            DIV_S|                   TY_AM64_R_R_R|   I32|
|"i32.div_u"|            DIV_U|                   TY_AM64_R_R_R|   I32|
|"i32.rem_s"|            REM_S|                   TY_AM64_R_R_R|   I32|
|"i32.rem_u"|            REM_U|                   TY_AM64_R_R_R|   I32|
|"i32.and"|              AND|                     TY_AM64_R_R_R|   I32|
|"i32.or"|               OR|                      TY_AM64_R_R_R|   I32|
|"i32.xor"|              XOR|                     TY_AM64_R_R_R|   I32|
|"i32.shl"|              SHL|                     TY_AM64_R_R_R|   I32|
|"i32.shr_s"|            SHR_S|                   TY_AM64_R_R_R|   I32|
|"i32.shr_u"|            SHR_U|                   TY_AM64_R_R_R|   I32|
|"i32.rotl"|             ROTL|                    TY_AM64_R_R_R|   I32|
|"i32.rotr"|             ROTR|                    TY_AM64_R_R_R|   I32|
|"i64.clz"|              CLZ|                     TY_AM64_R_R_R|   I64|
|"i64.ctz"|              CTZ|                     TY_AM64_R_R_R|   I64|
|"i64.popcnt"|           POPCNT|                  TY_AM64_R_R_R|   I64|
|"i64.add"|              ADD|                     TY_AM64_R_R_R|   I64|
|"i64.sub"|              SUB|                     TY_AM64_R_R_R|   I64|
|"i64.mul"|              MUL|                     TY_AM64_R_R_R|   I64|
|"i64.div_s"|            DIV_S|                   TY_AM64_R_R_R|   I64|
|"i64.div_u"|            DIV_U|                   TY_AM64_R_R_R|   I64|
|"i64.rem_s"|            REM_S|                   TY_AM64_R_R_R|   I64|
|"i64.rem_u"|            REM_U|                   TY_AM64_R_R_R|   I64|
|"i64.and"|              AND|                     TY_AM64_R_R_R|   I64|
|"i64.or"|               OR|                      TY_AM64_R_R_R|   I64|
|"i64.xor"|              XOR|                     TY_AM64_R_R_R|   I64|
|"i64.shl"|              SHL|                     TY_AM64_R_R_R|   I64|
|"i64.shr_s"|            SHR_S|                   TY_AM64_R_R_R|   I64|
|"i64.shr_u"|            SHR_U|                   TY_AM64_R_R_R|   I64|
|"i64.rotl"|             ROTL|                    TY_AM64_R_R_R|   I64|
|"i64.rotr"|             ROTR|                    TY_AM64_R_R_R|   I64|
|"f32.abs"|              ABS|                     TY_AM64_R_R_R|   F32|
|"f32.neg"|              NEG|                     TY_AM64_R_R_R|   F32|
|"f32.ceil"|             CEIL|                    TY_AM64_R_R_R|   F32|
|"f32.floor"|            FLOOR|                   TY_AM64_R_R_R|   F32|
|"f32.trunc"|            TRUNC|                   TY_AM64_R_R_R|   F32|
|"f32.nearest"|          NEAREST|                 TY_AM64_R_R_R|   F32|
|"f32.sqrt"|             SQRT|                    TY_AM64_R_R_R|   F32|
|"f32.add"|              ADD|                     TY_AM64_R_R_R|   F32|
|"f32.sub"|              SUB|                     TY_AM64_R_R_R|   F32|
|"f32.mul"|              MUL|                     TY_AM64_R_R_R|   F32|
|"f32.div"|              DIV_S|                   TY_AM64_R_R_R|   F32|
|"f32.min"|              MIN|                     TY_AM64_R_R_R|   F32|
|"f32.max"|              MAX|                     TY_AM64_R_R_R|   F32|
|"f32.copysign"|         COPYSIGN|                TY_AM64_R_R_R|   F32|
|"f64.abs"|              ABS|                     TY_AM64_R_R_R|   F64|
|"f64.neg"|              NEG|                     TY_AM64_R_R_R|   F64|
|"f64.ceil"|             CEIL|                    TY_AM64_R_R_R|   F64|
|"f64.floor"|            FLOOR|                   TY_AM64_R_R_R|   F64|
|"f64.trunc"|            TRUNC|                   TY_AM64_R_R_R|   F64|
|"f64.nearest"|          NEAREST|                 TY_AM64_R_R_R|   F64|
|"f64.sqrt"|             SQRT|                    TY_AM64_R_R_R|   F64|
|"f64.add"|              ADD|                     TY_AM64_R_R_R|   F64|
|"f64.sub"|              SUB|                     TY_AM64_R_R_R|   F64|
|"f64.mul"|              MUL|                     TY_AM64_R_R_R|   F64|
|"f64.div"|              DIV_S|                   TY_AM64_R_R_R|   F64|
|"f64.min"|              MIN|                     TY_AM64_R_R_R|   F64|
|"f64.max"|              MAX|                     TY_AM64_R_R_R|   F64|
|"f64.copysign"|         COPYSIGN|                TY_AM64_R_R_R|   F64|
|"i32.wrap_i64"|         WRAP_I64|                TY_AM64_R_R_R|   I32|
|"i32.trunc_f32_s"|      TRUNC_F32_S|             TY_AM64_R_R_R|   I32|
|"i32.trunc_f32_u"|      TRUNC_F32_U|             TY_AM64_R_R_R|   I32|
|"i32.trunc_f64_s"|      TRUNC_F64_S|             TY_AM64_R_R_R|   I32|
|"i32.trunc_f64_u"|      TRUNC_F64_U|             TY_AM64_R_R_R|   I32|
|"i64.extend_i32_s"|     EXTEND_I32_S|            TY_AM64_R_R_R|   I64|
|"i64.extend_i32_u"|     EXTEND_I32_U|            TY_AM64_R_R_R|   I64|
|"i64.trunc_f32_s"|      TRUNC_F32_S|             TY_AM64_R_R_R|   I64|
|"i64.trunc_f32_u"|      TRUNC_F32_U|             TY_AM64_R_R_R|   I64|
|"i64.trunc_f64_s"|      TRUNC_F64_S|             TY_AM64_R_R_R|   I64|
|"i64.trunc_f64_u"|      TRUNC_F64_U|             TY_AM64_R_R_R|   I64|
|"f32.convert_i32_s"|    CONVERT_I32_S|           TY_AM64_R_R_R|   F32|
|"f32.convert_i32_u"|    CONVERT_I32_U|           TY_AM64_R_R_R|   F32|
|"f32.convert_i64_s"|    CONVERT_I64_S|           TY_AM64_R_R_R|   F32|
|"f32.convert_i64_u"|    CONVERT_I64_U|           TY_AM64_R_R_R|   F32|
|"f32.demote_f64"|       DEMOTE_F64|              TY_AM64_R_R_R|   F32|
|"f64.convert_i32_s"|    CONVERT_I32_S|           TY_AM64_R_R_R|   F64|
|"f64.convert_i32_u"|    CONVERT_I32_U|           TY_AM64_R_R_R|   F64|
|"f64.convert_i64_s"|    CONVERT_I64_S|           TY_AM64_R_R_R|   F64|
|"f64.convert_i64_u"|    CONVERT_I64_U|           TY_AM64_R_R_R|   F64|
|"f64.promote_f32"|      PROMOTE_F32|             TY_AM64_R_R_R|   F64|
|"i32.reinterpret_f32"|  REINTERPRET_F32|         TY_AM64_R_R_R|   I32|
|"i64.reinterpret_f64"|  REINTERPRET_F64|         TY_AM64_R_R_R|   I64|
|"f32.reinterpret_i32"|  REINTERPRET_I32|         TY_AM64_R_R_R|   F32|
|"f64.reinterpret_i64"|  REINTERPRET_I64|         TY_AM64_R_R_R|   F64|
|"i32.mov"|              MOV|                     TY_AM32_R_R|     I32|
|"i64.mov"|              MOV|                     TY_AM32_R_R|     I64|
|"f32.mov"|              MOV|                     TY_AM32_R_R|     F32|
|"f64.mov"|              MOV|                     TY_AM32_R_R|     F64|
