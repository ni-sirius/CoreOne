#include "stdafx.h"
#include "Render.h"

#include "Material.h"
#include "Mesh.h"

void updateInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void updateInput(GLFWwindow* window, Mesh& mesh)
{
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    mesh.Move(glm::vec3(0.f, 0.f, -0.01f));
  }
  else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    mesh.Move(glm::vec3(0.f, 0.f, 0.01f));
  }
  else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    mesh.Move(glm::vec3(-0.01f, 0.f, 0.f));
  }
  else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    mesh.Move(glm::vec3(0.01f, 0.f, 0.f));
  }
  else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    mesh.Rotate(glm::vec3(0.f, -0.5f, 0.f));
  }
  else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    mesh.Rotate(glm::vec3(0.f, 0.5f, 0.f));
  }
  else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
  {
    mesh.Upscale(glm::vec3(-0.01f));
  }
  else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
  {
    mesh.Upscale(glm::vec3(0.01f));
  }
}

GLFWwindow* createWindow(std::string title,
                         const int width, const int height,
                         int& fbWidth, int& fbHeight,
                         int GLMajorVer, int GLMinorVer,
                         bool resizable)
{
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLMajorVer);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLMinorVer);
  glfwWindowHint(GLFW_RESIZABLE, resizable);

  GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

  glfwGetFramebufferSize(window, &fbWidth, &fbHeight); //for prrojection matrix

  auto framebufferResizeCallback = [](GLFWwindow* window, int frameBufWidth, int frameBufHeight) {
    glViewport(0, 0, frameBufWidth, frameBufHeight);
  };
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
  //glViewport(0, 0, framebufferWidth, frameBufferHeight); if static size

  glfwMakeContextCurrent(window);

  return window;
}

void RenderLoop()
{  //Init GLFW
  glfwInit();

  //Create window
  const int GLMajorVer = 4;
  const int GLMinorVer = 5;
  const size_t WINDOW_WIDTH = 640;
  const size_t WINDOW_HEIGHT = 480;
  int framebufferWidth = 0;
  int frameBufferHeight = 0;

  GLFWwindow* window = createWindow("OpenGL Test App",
                                    WINDOW_WIDTH, WINDOW_HEIGHT,
                                    framebufferWidth, frameBufferHeight,
                                    GLMajorVer, GLMinorVer,
                                    false);

  //Init GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "ERROR::MAIN GLEW_Init_Failed" << std::endl;
    glfwTerminate();
  }

  //OpenGL enable options
  glEnable(GL_DEPTH_TEST);

  //TODO learn culling and uncomment
  //glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //Shader init
  Shader coreProgram(GLMajorVer, GLMinorVer,"vertex_core.glsl", "fragment_core.glsl");

  //MODEL
  Mesh testMesh(&Quad(), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(2.f));

  //Texturing
  Texture texture0("Images/apple.png", GL_TEXTURE_2D, 0);
  Texture texture1("Images/flower.png", GL_TEXTURE_2D, 1);

  //Material
  Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.GetTextureUnit(), texture1.GetTextureUnit());

  //Matrices
  glm::vec3 camPosition(0.f, 0.f, 1.f);
  glm::vec3 worldUp(0.f, 1.f, 0.f);
  glm::vec3 camFront(0.f, 0.f, -1.f);
  glm::mat4 ViewMatrix(1.f);
  ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

  float fov = 90.f;
  float nearPlane = 0.1f;
  float farPlane = 1000.f;
  glm::mat4 ProjectionMatrix(1.f);
  ProjectionMatrix = glm::perspective(
                                      glm::radians(fov),
                                      static_cast<float>(framebufferWidth) / frameBufferHeight,
                                      nearPlane,
                                      farPlane);

  //Lights
  glm::vec3 lightPos0(0.f, 0.f, 0.5f);

  //Init uniforms
  coreProgram.SetMat4fv(ViewMatrix, "ViewMat");
  coreProgram.SetMat4fv(ProjectionMatrix, "ProjectionMat");

  coreProgram.SetVec3f(lightPos0, "LightPos0");
  coreProgram.SetVec3f(camPosition, "CameraPos");

  //Main loop
  while (!glfwWindowShouldClose(window))
  {
    //update input
    glfwPollEvents();
    updateInput(window, testMesh);

    //update Exit
    updateInput(window);

    //draw
    //clear
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Update uniforms
    coreProgram.Set1i(texture0.GetTextureUnit(), "texture0");
    coreProgram.Set1i(texture1.GetTextureUnit(), "texture1");
    material0.SendToShader(coreProgram);

    //To handle resizing of the window
    glfwGetFramebufferSize(window, &framebufferWidth, &frameBufferHeight);
    ProjectionMatrix = glm::perspective(
      glm::radians(fov),
      static_cast<float>(framebufferWidth) / frameBufferHeight,
      nearPlane,
      farPlane);
    coreProgram.SetMat4fv(ProjectionMatrix, "ProjectionMat");

    //use a program
    coreProgram.Use();

    //Activate texture
    texture0.Bind();
    texture1.Bind();

    //draw

    //Draw Mesh
    testMesh.Render(&coreProgram);

    //end draw
    glfwSwapBuffers(window);
    glFlush();

    //free
    coreProgram.Unuse();

    glBindVertexArray(0);
    texture0.Unbind();
    texture1.Unbind();//Not necessary actually
  }

  //Ending
  glfwDestroyWindow(window);
  glfwTerminate();
}