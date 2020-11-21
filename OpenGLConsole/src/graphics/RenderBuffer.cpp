#include "stdafx.h"
#include "graphics/RenderBuffer.h"

using namespace coreone::graphics;

RenderBuffer::RenderBuffer(Format format, unsigned int width, unsigned int height)
{
  CreateRenderBuffer(format, width, height);
}

RenderBuffer::~RenderBuffer()
{
  if (_valid)
  {
    glDeleteRenderbuffers(1, &_id);
  }
}

void RenderBuffer::CreateRenderBuffer(Format format, unsigned int width, unsigned int height)
{
  if (_id)
  {
    glDeleteRenderbuffers(1, &_id);
  }

  glGenRenderbuffers(1, &_id);
  glBindRenderbuffer(GL_RENDERBUFFER, _id);

  if (format == Format::COLOR)
  {
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);

    _valid = true;
  }
  else if (format == Format::DEPTH_STENCIL)
  {
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    _valid = true;
  }

  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

GLuint RenderBuffer::GetID() const
{
  return _id;
}

void RenderBuffer::Bind()
{
  glBindRenderbuffer(GL_RENDERBUFFER, _id);
}

void RenderBuffer::Unbind()
{
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
