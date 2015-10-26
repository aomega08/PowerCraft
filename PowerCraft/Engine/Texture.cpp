#include "../stdafx.h"
#include "Texture.h"

Texture::Texture(std::string _path, int type) {
  glGenTextures(1, &_id);

  unsigned char* image = SOIL_load_image(_path.c_str(), &width, &height, 0, type == GL_RGB ? SOIL_LOAD_RGB : SOIL_LOAD_RGBA);

  // We are writing on the texture object. We have to bind it first
  Bind();

  // Repeat on wrap and use nearest interpolation
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Move the raw pixels into the texture
  glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Release the raw bytes array and unbind the texture
  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
  glDeleteTextures(1, &_id);
}

void Texture::Bind(int unit) {
  glActiveTexture(unit);
  glBindTexture(GL_TEXTURE_2D, _id);
}
