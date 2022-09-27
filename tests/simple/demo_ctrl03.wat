(module
  (type $t0 (func))
  (type $t1 (func (param i32 i32 i32) (result i32)))
  (func $add (export "add") (type $t1) (param $p0 i32) (param $p1 i32) (param $p2 i32) (result i32)
    (local $l0 i32)
    (local $l1 i32)
    i32.const 10
    set_local $p2
    i32.const -2
    set_local $p0
    i32.const 9
    set_local $p1
    i32.const 2
    set_local $l1
    i32.const 0
    set_local $l0
    loop $L0
      get_local $p0
      get_local $p1
      i32.add
      tee_local $p1
      i32.const 0
      i32.ne
      if $I1
        get_local $p1
        get_local $l1
        i32.div_s
        set_local $p0
      else
        get_local $p1
        get_local $l1
        i32.mul
        set_local $p0
      end
      get_local $l0
      i32.const 1
      i32.add
      tee_local $l0
      get_local $p2
      i32.lt_s
      if $I2
        br $L0
      end
    end
    get_local $p0)
  (func $fa (type $t0)
       call $fb)

  (func $fb (type $t0)
       call $fc)

  (func $fc (type $t0)
       call $fd
       call $fe)

  (func $fd (type $t0) )
  (func $fe (type $t0) )
  (memory $memory (export "memory") 0)
  (global $q (export "q") (mut i32) (i32.const 0))
  (start 2))
