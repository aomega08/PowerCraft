#include "../stdafx.h"
#include "Shader.h"

Shader::Shader(std::string path) : _path(path) {
}

Shader::~Shader() {
  glDeleteShader(_id);
}

void Shader::Compile() {
  std::ifstream file(_path);
  if (file.fail()) {
    throw std::string("Unable to read shader file: ") + _path;
  }

  std::string source;

  file.seekg(0, std::ios::end);
  source.reserve(file.tellg());
  file.seekg(0, std::ios::beg);

  source.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  const char *ptr = source.c_str();

  glShaderSource(_id, 1, &ptr, NULL);
  glCompileShader(_id);

  GLint status;
  glGetShaderiv(_id, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE) {
    char buffer[512];
    glGetShaderInfoLog(_id, 512, NULL, buffer);

    throw std::string("Shader compilation failed: ") + buffer;
  }
}
