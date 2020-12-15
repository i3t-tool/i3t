
#include <stdio.h>
#include <iostream>  // cerr cout

#include "pgr.h"

namespace pgr {

GLuint createShaderFromSource(GLenum eShaderType, const std::string &strShaderText) {
  GLuint shader = glCreateShader(eShaderType);
  const char *strFileData = strShaderText.c_str();
  glShaderSource(shader, 1, &strFileData, NULL);

  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

    const char *strShaderType = NULL;
    switch(eShaderType) {
      case GL_VERTEX_SHADER: strShaderType   = "vertex";   break;
      case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
      case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
    }

    std::cerr << "Compile failure in " << strShaderType << " shader:" << std::endl;
    std::cerr << strInfoLog;

    delete[] strInfoLog;
    glDeleteShader(shader);
    return 0;
  }

  CHECK_GL_ERROR();
  return shader;
}

GLuint createShaderFromFile(GLenum eShaderType, const std::string &filename) {
  FILE *f = fopen(filename.c_str(), "rb");
  if (!f) {
    std::cerr << "Unable to open file " << filename << " for reading" << std::endl;
    return 0;
  } else
    std::cout << "loading shader: " << filename << std::endl;

  fseek(f, 0, SEEK_END);
  int length = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *buffer = new char[length + 1];
  fread(buffer, 1, length, f);
  fclose(f);
  buffer[length] = '\0';

  GLuint sh = createShaderFromSource(eShaderType, buffer);
  delete [] buffer;
  return sh;
}

static bool linkProgram(GLuint program) {
  glLinkProgram(program);

  GLint status = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  CHECK_GL_ERROR();

  if(status == GL_FALSE) {
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
    fprintf(stderr, "Linker failure: %s\n", strInfoLog);
    delete[] strInfoLog;
    return false;
  }

  return true;
}

GLuint createProgram(const GLuint * shaders) {
  GLuint program = glCreateProgram();

  while(shaders && *shaders)
    glAttachShader(program, *shaders++);
  CHECK_GL_ERROR();

  if(!linkProgram(program)) {
    deleteProgramAndShaders(program);
    return 0;
  }

  return program;
}

GLuint createProgram(const std::vector<GLuint> &shaderList) {
  GLuint program = glCreateProgram();

  for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
    glAttachShader(program, shaderList[iLoop]);
  CHECK_GL_ERROR();

  if(!linkProgram(program)) {
    deleteProgramAndShaders(program);
    return 0;
  }

  return program;
}

void deleteProgramAndShaders(GLuint program) {
  const GLsizei MAX_COUNT = 10;  // Pisvejc's constant
  GLsizei count = 0;
  GLuint shaders[MAX_COUNT];

  if(program != 0) {
    glGetAttachedShaders( program, MAX_COUNT, &count, shaders );
    for( int i=0; i<count; i++)
      glDeleteShader( shaders[i] );

    glDeleteProgram( program );
  }
}

} // end namespace pgr
