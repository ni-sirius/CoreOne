#pragma once
#include <string>

#include <glew.h>
#include <glfw3.h>

class Texture
{
public:
  Texture(std::string path, GLenum type);
  ~Texture();

  void LoadFromFile(std::string path);

  GLuint GetID() const;

  void Bind(GLint textureUnit);
  void Unbind();

private:
  GLuint _id;
  int _width;
  int _height;

  unsigned int _type;
};
