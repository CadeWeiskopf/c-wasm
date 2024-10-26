#include <stdio.h>
#include <emscripten.h>
#include <GLES2/gl2.h>
#include <emscripten/html5.h>

void render() {
  glClear(GL_COLOR_BUFFER_BIT);
  printf("render\n");
}

int main() {
  EmscriptenWebGLContextAttributes attrs;
  attrs.alpha = true;
  attrs.depth = true;
  attrs.stencil = true;
  attrs.antialias = true;
  attrs.premultipliedAlpha = false;
  attrs.preserveDrawingBuffer = false;
  attrs.failIfMajorPerformanceCaveat = false;
  attrs.majorVersion = 1;
  attrs.minorVersion = 0;
  attrs.enableExtensionsByDefault = false;

  const char *canvas_id = "canvas";
  int ctx = emscripten_webgl_create_context(canvas_id, &attrs);
  if (!ctx) {
    printf("webgl ctx not created\n");
    return -1;
  }    

  emscripten_webgl_make_context_current(ctx);

  glClearColor(1, 0, 1, 0.1);

  emscripten_set_main_loop(render, 0, 1);
  
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