//
// Created by xc5 on 2020/7/19.
//

#define WASM_EXPORT __attribute__((visibility("default")))

WASM_EXPORT
int a;

WASM_EXPORT
int b;

WASM_EXPORT
int gain();

WASM_EXPORT
int use(int a);

WASM_EXPORT
int func(int m, int n) {
  //for (int i = 0; i < m; i++ ) {
  while(1) {
    if (a < m) {
      break;
    }
    a = a + a + n;
    gain();
  }
}

WASM_EXPORT
int main() {
  func(gain(), gain());
}
