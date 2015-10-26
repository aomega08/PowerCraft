#pragma once

#include "../stdafx.h"

class VertexBufferObject {
public:
  VertexBufferObject();
  ~VertexBufferObject();

  void Bind();
  void Upload(void *data, size_t size, int type = GL_STATIC_DRAW);

private:
  GLuint _id;
};
