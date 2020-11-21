#include "stdafx.h"
#include "graphics/Texture.h"

using namespace coreone::graphics;

Texture::Texture(GLenum type, Format format, unsigned int width, unsigned int height)
{
  CreateTexture(type, format, width, height);
}

Texture::Texture(std::string path, GLenum type)
{
  LoadTexture(path, type);
}

Texture::~Texture()
{
  if (_valid)
  {
    glDeleteTextures(1, &_id);
  }
}

void Texture::CreateTexture(GLenum type, Format format, unsigned int width, unsigned int height)
{
  if (_id)
  {
    glDeleteTextures(1, &_id);
  }

  _type = type;

  glGenTextures(1, &_id);
  glBindTexture(_type, _id);

  if (format == Format::COLOR)
  {
    glTexParameteri(_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(_type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glGenerateMipmap(_type);

    _valid = true;
  }
  else if (format == Format::DEPTH_STENCIL)
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(_type, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    _valid = true;
  }

  glBindTexture(_type, 0);
}

void Texture::LoadTexture(std::string path, GLenum type)
{
  if (_id)
  {
    glDeleteTextures(1, &_id);
  }

  _type = type;

  int width, height;
  unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, NULL, SOIL_LOAD_RGBA);

  glGenTextures(1, &_id);
  glBindTexture(_type, _id);

  glTexParameteri(_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  if (image)
  {
    glTexImage2D(_type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(_type);
  }
  else
  {
    std::cout << "ERROR::TEXTURE on path " << path << " load failed" << std::endl;
  }

  glActiveTexture(0);
  glBindTexture(GL_TEXTURE_2D, 0);
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
  glBindTexture(_type, 0);
  glActiveTexture(0);
}
