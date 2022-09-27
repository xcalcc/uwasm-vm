(module
  (type $t0 (func (param i32) (result i32)))
  (type $t1 (func))
  (type $t2 (func (result i32)))
  (import "env" "bar" (func $bar (type $t0)))
  (func $__wasm_call_ctors (type $t1))
  (func $main (export "main") (type $t2) (result i32)
    (local $l0 i32) (local $l1 i32)
    i32.const 3
    call $bar
    set_local $l0
    block $B0
      i32.const 0
      i32.load offset=1024
      i32.const 1
      i32.lt_s
      br_if $B0
      i32.const 0
      set_local $l1
      loop $L1
        i32.const 100
        get_local $l1
        i32.const 1
        i32.add
        tee_local $l1
        i32.const 0
        i32.load offset=1024
        i32.lt_s
        br_if $L1
      end
    end
    get_local $l0)
  (table $T0 1 1 anyfunc)
  (memory $memory (export "memory") 2)
  (global $g0 (mut i32) (i32.const 66576))
  (global $__heap_base (export "__heap_base") i32 (i32.const 66576))
  (global $__data_end (export "__data_end") i32 (i32.const 1028))
  (global $c (export "c") i32 (i32.const 1024))
  (data (i32.const 1024) "\00\00\00\00"))
