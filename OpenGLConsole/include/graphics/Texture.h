#pragma once
#include <string>

#include <glew.h>
#include <glfw3.h>

namespace coreone::graphics
{
  class Texture
  {
  public:
    enum Format {COLOR = 0, DEPTH_STENCIL};

    Texture() = default;
    Texture(GLenum type, Format format, unsigned int width, unsigned int height);
    Texture(std::string path, GLenum type);
    ~Texture();

    void CreateTexture(GLenum type, Format format, unsigned int width, unsigned int height);
    void LoadTexture(std::string path, GLenum type);

    GLuint GetID() const;

    void Bind(GLint textureUnit);
    void Unbind();

    bool Valid() { return _valid; }

  private:
    bool _valid = false;

    GLuint _id;
    unsigned int _type;
    GLint _currentTexUnit;
  };
}
