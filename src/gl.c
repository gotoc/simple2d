// OpenGL Shared Functions

#include "../include/simple2d.h"

// For GL3 and GLES, an orthographic 2D projection matrix.
// Matrix is given in column-first order.
GLfloat orthoMatrix[16] = {
  2.0f, 0, 0, 0,
  0, -2.0f, 0, 0,
  0, 0, -2.0f / 128.0, 0,  // 128.0 == far_z
  -1.0f, 1.0f, -1.0f, 1.0f
};


/*
 * Prints current GL error
 */
void gl_print_error(char *error) {
  printf("%s: %d\n", error, glGetError());
}


/*
 * Print info about the current OpenGL context
 */
void gl_print_context_info(Window *window) {
  puts("\n--- OpenGL Context ---");
  printf("GL_VENDOR: %s\n", window->S2D_GL_VENDOR);
  printf("GL_RENDERER: %s\n", window->S2D_GL_RENDERER);
  printf("GL_VERSION: %s\n", window->S2D_GL_VERSION);
  printf("GL_MAJOR_VERSION: %i\n", window->S2D_GL_MAJOR_VERSION);
  printf("GL_MINOR_VERSION: %i\n", window->S2D_GL_MINOR_VERSION);
  printf("GL_SHADING_LANGUAGE_VERSION: %s\n", window->S2D_GL_SHADING_LANGUAGE_VERSION);
  puts("");
}


/*
 * Store info about the current OpenGL context
 */
void gl_store_context_info(Window *window) {
  window->S2D_GL_VENDOR = glGetString(GL_VENDOR);
  window->S2D_GL_RENDERER = glGetString(GL_RENDERER);
  window->S2D_GL_VERSION = glGetString(GL_VERSION);
  
  #if !GLES
    glGetIntegerv(GL_MAJOR_VERSION, &window->S2D_GL_MAJOR_VERSION);
    glGetIntegerv(GL_MINOR_VERSION, &window->S2D_GL_MINOR_VERSION);
  #endif
  
  window->S2D_GL_SHADING_LANGUAGE_VERSION = glGetString(GL_SHADING_LANGUAGE_VERSION);
};


/*
 * Creates a shader object, loads shader string, and compiles.
 * Returns 0 if shader could not be compiled.
 */
GLuint gl_load_shader(GLenum type, const GLchar *shaderSrc, char *shaderName) {
  
  GLuint shader;
  GLint compiled;
  
  // Create the shader object
  shader = glCreateShader(type);
  
  if (shader == 0) {
    gl_print_error("Failed to create shader program");
    return 0;
  }
  
  // Load the shader source
  glShaderSource(shader, 1, &shaderSrc, NULL);
  
  // Compile the shader
  glCompileShader(shader);
  
  // Check the compile status
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  
  if (!compiled) {
    
    GLint infoLen = 0;
    
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    
    if (infoLen > 1) {
      
      char* infoLog = malloc(sizeof(char)*infoLen);
      
      glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
      printf("Error compiling shader %s:\n%s\n", shaderName, infoLog);
      
      free(infoLog);
    }
    
    glDeleteShader(shader);
    return 0;
  }
  
  return shader;
}
