(module
  (memory 1)
  (func $local1 (export "local1") (param $p0 i32) (param $p1 i32) (param $p2 i64) (result i32)
    (local $l0 i32) (local $l1 i32) (local $l2 i64) (local $l3 i32) (local $l4 i32) (local $l5 i32) (local $l6 i64) (local $l7 i64)
    get_local $l0
    get_local $l1
    i32.div_s
    set_local $l1
    get_local $l1
    tee_local $l0
  )
  (func $local2 (export "local2") (param $p0 i32) (result i64)
    (local $l0 i32) (local $l1 i32) (local $l2 i64) (local $l3 i32) (local $l4 i32) (local $l5 i32) (local $l6 i64) (local $l7 i64)
    get_local $l2
  )
  (func $local3 (export "local3") (param $p0 i32) (param $p1 i64) (result i32)
    (local $l0 i32) (local $l1 i32) (local $l2 i64) (local $l3 i32) (local $l4 i32) (local $l5 i32) (local $l6 i64) (local $l7 i64)
    get_local $l3
  )
  (func $local4 (export "local4") (param $p0 i32) (param $p1 i64) (result i32)
    (local $l0 i32) (local $l1 i32)
    get_local $l1
  )
  (func $local5 (export "local5") (param $p0 i32) (param $p1 i64) (result i32)
    (local $l0 i32)
    get_local $l0
  )
)