#pragma once

#include "../stdafx.h"
#include "Shader.h"
#include "VertexArrayObject.h"

class ShaderProgram {
public:
  ShaderProgram();
  ~ShaderProgram();

  void Attach(Shader &shader);
  void Link();

  void Use();

  void SetupAttribute(std::string attribute, int count, int type, int normalized, int stride = 0, int offset = 0);

  int GetUniformId(std::string name);

private:
  GLuint _id;
  VertexArrayObject _vao;
};
