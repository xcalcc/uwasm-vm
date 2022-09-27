export function fib(n: i32): i32 {
  if (n <= 2) {
    return 1;
  }
  return bar(n-1) + bar(n-2);
}

export function bar(n : i32) : i32 {
  if (n <= 2) {
    return 1;
  }
  return fib(n-1) + fib(n-2);
}




