#pragma once

#include "stdafx.h"

class Shader {
public:
  Shader(std::string path);
  ~Shader();

  void Compile();

protected:
  GLuint _id;

private:
  std::string _path;
};
