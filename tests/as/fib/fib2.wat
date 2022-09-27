(module
  (type (;0;) (func (param i32) (result i32)))
  (func $fib (type 0) (param i32) (result i32)
    block  ;; label = @1
      local.get 0
      i32.const 2
      i32.le_s
      if  ;; label = @2
        i32.const 1
        return
      end
      local.get 0
      i32.const 1
      i32.sub
      call $fib
      local.get 0
      i32.const 2
      i32.sub
      call $fib
      i32.add
      return
      unreachable
    end
    unreachable)
  (memory (;0;) 1)
  (export "fib" (func $fib))
  (export "memory" (memory 0)))
