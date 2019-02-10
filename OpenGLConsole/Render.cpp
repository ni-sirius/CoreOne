#include "stdafx.h"
#include "Render.h"

#include "Core.h"


void RenderLoop()
{
  Core coreObject("OpenGL Test App", 640, 480, 4, 5, false);

  //Main loop
  while (!coreObject.GetWindiwShouldClose())
  {
    //update input
    coreObject.Update();
    coreObject.Render();

    //Clear up all
    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}