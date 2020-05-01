#include "stdafx.h"

#include "Texture.h"

Texture::Texture(std::string path, GLenum type):
  _type(type),
  _currentTexUnit(0)
{
  LoadFromFile(path);
}

Texture::~Texture()
{
  glDeleteTextures(1, &_id);
}

void Texture::LoadFromFile(std::string path)
{
  if (_id)
  {
    glDeleteTextures(1, &_id);
  }

  unsigned char* image = SOIL_load_image(path.c_str(), &_width, &_height, NULL, SOIL_LOAD_RGBA);

  glGenTextures(1, &_id);
  glBindTexture(_type, _id);

  glTexParameteri(_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  if (image)
  {
    glTexImage2D(_type, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(_type);
  }
  else
  {
    std::cout << "ERROR::TEXTURE on path " << path << " load failed" << std::endl;
  }

  glActiveTexture(0);
  glBindTexture(GL_TEXTURE_2D, 0); // set text to 0, because if all goes wrong there is random data. insted noeow
  SOIL_free_image_data(image);
}

GLuint Texture::GetID() const
{
  return _id;
}

void Texture::Bind(GLint textureUnit)
{
  _currentTexUnit = textureUnit;

  glActiveTexture(GL_TEXTURE0 + _currentTexUnit);
  glBindTexture(_type, _id);
}

void Texture::Unbind()
{
  glActiveTexture(GL_TEXTURE0 + _currentTexUnit);
  glBindTexture(_type, 0);
  glActiveTexture(0);
}
