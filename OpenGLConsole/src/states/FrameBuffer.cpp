#include "stdafx.h"
#include <states/FrameBuffer.h>

using namespace core;

FrameBuffer::FrameBuffer()
{
  glGenFramebuffers(1, &_framebuffer);
}

FrameBuffer::~FrameBuffer()
{
  glDeleteFramebuffers(1, &_framebuffer);
}

void FrameBuffer::AddTextureAttachement(const Texture& texture)
{
  GLint drawFboId = 0, readFboId = 0;
  glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
  glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFboId);//check

  glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
}

void FrameBuffer::SetUpAttachements()
{

}

void FrameBuffer::Use()
{

}

void FrameBuffer::Unuse()
{

}


