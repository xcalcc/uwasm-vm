(module
  (func $i (import "imports" "imported_func") (param i32))
  (func (export "exported_func") (param i32) (result i32)
    local.get 0
    i32.const 42
    i32.add
  )
)
