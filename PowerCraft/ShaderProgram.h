#pragma once

#include "stdafx.h"
#include "Shader.h"

class ShaderProgram {
public:
  ShaderProgram();
  ~ShaderProgram();

  void Attach(Shader &shader);
  void Link();

  void Use();

private:
  GLuint _id;
};
