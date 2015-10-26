#include "../stdafx.h"
#include "VertexShader.h"

VertexShader::VertexShader(std::string path) : Shader(path) {
  _id = glCreateShader(GL_VERTEX_SHADER);
}
