#include <stdafx.h>
#include "FragmentShader.h"

FragmentShader::FragmentShader(std::string path) : Shader(path) {
  _id = glCreateShader(GL_FRAGMENT_SHADER);
}
