#include <stdio.h>
#include <emscripten.h>

int main() {
  printf("c\n");
  return 0;
}

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

EXTERN EMSCRIPTEN_KEEPALIVE void test() {
  printf("test\n");
}