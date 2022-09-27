(module
  (memory 1)
  (func (export "i32_mem") (result i32)
	(i32.sub (i32.const 4095) (i32.const 4096) )  ;; 4095, 4096.
  )
)