#pragma once
#include <string>

#include <glew.h>
#include <glfw3.h>

namespace coreone::graphics
{
  class RenderBuffer
  {
  public:
    enum Format { COLOR = 0, DEPTH_STENCIL };

    RenderBuffer() = default;
    RenderBuffer(Format format, unsigned int width, unsigned int height);
    ~RenderBuffer();

    void CreateRenderBuffer(Format format, unsigned int width, unsigned int height);

    GLuint GetID() const;

    void Bind();
    void Unbind();

    bool Valid() { return _valid; }

  private:
    bool _valid = false;

    GLuint _id;
  };
}
