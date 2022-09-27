# Faq

Why did we choose not to use the 'R0' and 'R1' as fixed integer 0 or 1?

- For following reasons:
  - 1. For virtual execution, differentiating the constant registers and normal registers puts a burden on the execution each instruction.
  - 2. If no execution side burden, then we need to allocate these registers in function prologue and set them to the values purposefully, this has a performance drawback.
