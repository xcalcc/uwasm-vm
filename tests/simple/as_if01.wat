(module
  (type $t0 (func))
  (type $t1 (func (param i32 i32) (result i32)))
  (import "main" "sayHello" (func $main.sayHello (type $t0)))
  (func $add (export "add") (type $t1) (param $p0 i32) (param $p1 i32) (result i32)
    get_local $p0
    get_local $p1
    i32.gt_s
    if $I0
      get_local $p0
      get_local $p1
      i32.add
      set_global $q
    end
    get_local $p0
    get_local $p1
    i32.div_s)
  (func $f2 (type $t0)
    call $main.sayHello)
  (memory $memory (export "memory") 0)
  (global $q (export "q") (mut i32) (i32.const 0))
  (start 2))
