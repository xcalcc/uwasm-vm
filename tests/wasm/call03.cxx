//
// Created by xc5 on 2020/11/16.
//

// using an external function
extern int rand();
extern int add(long a, int b);

int foo(int a, long b, int c) {
  return a + add(b, c);
}

int main() {
  int a = rand();
  long b = rand();
  int c = rand();
  return foo(a,b,c);
}