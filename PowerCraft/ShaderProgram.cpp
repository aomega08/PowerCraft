#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {
  _id = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
  glDeleteProgram(_id);
}

void ShaderProgram::Attach(Shader &shader) {
  glAttachShader(_id, shader._id);
}

void ShaderProgram::Link() {
  glLinkProgram(_id);
}

void ShaderProgram::Use() {
  glUseProgram(_id);
}
