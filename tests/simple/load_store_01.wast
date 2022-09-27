(module
  (memory 1)
  (func (export "i32_mem") (result i32)
    (i32.store (i32.const 8) (i32.const 1))
	(i32.load (i32.const 8))
    )
)