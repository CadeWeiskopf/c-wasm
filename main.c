#include <stdio.h>
#include <emscripten.h>
#include <GLES2/gl2.h>
#include <emscripten/html5.h>

const char *vertexShaderSource = "#version 100\n"
  "attribute vec3 aPos;\n"
  "void main() {\n"
  " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

const char *fragmentShaderSource = "#version 100\n"
  "precision mediump float;\n"
  "void main() {\n"
  " gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
  "}\0";

void render() {
  // glClear(GL_COLOR_BUFFER_BIT);

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


  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
  };
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
  }

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
      char infoLog[512];
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
  }

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::LINK_FAILED: %s\n", infoLog);
  }

  glUseProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glDrawArrays(GL_TRIANGLES, 0, 3);

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