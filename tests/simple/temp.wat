(module
  (type (;0;) (func (result i32)))
  (func (;0;) (type 0) (result i32)
    i32.const 8
    i32.const 1
    i32.store
    i32.const 8
    i32.load)
  (memory (;0;) 1)
  (export "i32_mem" (func 0)))
