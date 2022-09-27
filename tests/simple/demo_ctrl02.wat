(module
  (type $t0 (func))
  (type $t1 (func (param i32 i32) (result i32)))
  (import "main" "sayHello" (func $main.sayHello (type $t0)))
  (func $add (export "add") (type $t1) (param $p0 i32) (param $p1 i32) (result i32)
    (local $l0 i32)
    loop $L0
      i32.const 9
      i32.const 2
      i32.add
      if $I1
        i32.const 3
        i32.const 1
        i32.add
        set_local $p1
        loop $L2
          loop $L3
            i32.const 3
            i32.const 2
            i32.add
            set_local $p0
            br $L0
          end
        end
      end
      i32.const 9
      i32.const 2
      i32.add
      set_local $p1
    end
    i32.const 30)
  (func $f2 (type $t0)
    call $main.sayHello)
  (memory $memory (export "memory") 0)
  (global $q (export "q") (mut i32) (i32.const 0))
  (start 2))
