(module
  (memory 1)
  (func $A (export "A") (param $p0 i32) (param $p1 i32) (param $p2 i64) (result i32)
    (local $l0 i32) (local $l1 i32) (local $l2 i64) (local $l3 i32) (local $l4 i32) (local $l5 i32) (local $l6 i64) (local $l7 i64)
    get_local $l0
    get_local $l1
    i32.div_s
    call $B
    set_local $l2
    get_local $l1
    tee_local $l0
  )
  (func $B (export "B") (param $p0 i32) (result i64)
    (local $l0 i32)
    get_local $p0
    i64.const 20
    call $C
    return
  )
  (func $C (export "C") (param $p0 i32) (param $p1 i64) (result i64)
    (local $l0 i32)
    get_local $p0
    get_local $p1
    call $D
    return
  )
  (func $D (export "D") (param $p0 i32) (param $p1 i64) (result i64)
    (local $l0 i32) (local $l1 i32)
    i64.const 123
    return
  )
)