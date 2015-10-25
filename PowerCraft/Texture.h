#pragma once

#include "stdafx.h"

class Texture {
public:
  Texture(std::string _path, int type = GL_RGB);
  ~Texture();

  void Bind(int unit = GL_TEXTURE0);

private:
  GLuint _id;

  int width, height;
};
