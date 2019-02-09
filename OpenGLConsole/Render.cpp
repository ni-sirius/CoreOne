#include "stdafx.h"
#include "Render.h"

#include "Material.h"
#include "Mesh.h"

Vertex vertices[] = {
  //position                     //color                    //uvcoords               //normals                 
  glm::vec3(-0.5f, 0.5f, 0.0f),  glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f),      glm::vec3(0.f, 0.f, 1.f),
  glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f),      glm::vec3(0.f, 0.f, 1.f),
  glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),      glm::vec3(0.f, 0.f, 1.f),
  glm::vec3(0.5f, 0.5f, 0.0f),   glm::vec3(1.f, 1.f, 0.f), glm::vec2(1.f, 1.f),      glm::vec3(0.f, 0.f, 1.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = {
  0, 1, 2,
  0, 2, 3
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

void updateInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void updateInput(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    position.z -= 0.01f;
  }
  else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    position.z += 0.01f;
  }
  else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    position.x -= 0.01f;
  }
  else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    position.x += 0.01f;
  }
  else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    rotation.y -= 0.5f;
  }
  else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    rotation.y += 0.5f;
  }
  else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
  {
    scale -= 0.01f;
  }
  else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
  {
    scale += 0.01f;
  }
}

GLuint createTexture(std::string path)
{
  int imageWidth(0);
  int imageHeight(0);
  unsigned char* image = SOIL_load_image(path.c_str(), &imageWidth, &imageHeight, NULL, SOIL_LOAD_RGBA);

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);

  if (image)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Error::Texture on path " << path << " load failed" << std::endl;
  }

  glActiveTexture(0);
  glBindTexture(GL_TEXTURE_2D, 0); // set text to 0, because if all goes wrong there is random data. insted noeow
  SOIL_free_image_data(image);

  return texture;
}


void RenderLoop()
{  //Init GLFW
  glfwInit();

  //Create window
  const size_t WINDOW_WIDTH = 640;
  const size_t WINDOW_HEIGHT = 480;
  int framebufferWidth = 0;
  int frameBufferHeight = 0;

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OGL Test", nullptr, nullptr);

  glfwGetFramebufferSize(window, &framebufferWidth, &frameBufferHeight); //for prrojection matrix

  auto framebufferResizeCallback = [](GLFWwindow* window, int frameBufWidth, int frameBufHeight) {
    glViewport(0, 0, frameBufWidth, frameBufHeight);
  };
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
  //glViewport(0, 0, framebufferWidth, frameBufferHeight); if static size

  glfwMakeContextCurrent(window);

  //Init GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "ERROR MAIN.cpp GLEW_Init_Failed" << std::endl;
    glfwTerminate();
  }

  //OpenGL enable options
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //Shader init
  Shader coreProgram("vertex_core.glsl", "fragment_core.glsl");

  //MODEL
  Mesh testMesh(vertices, nrOfVertices, indices, nrOfIndices);

  //VAO VBO EBO, Generation of
  GLuint VAO; //Vertex array object
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  GLuint VBO; //Vertex buffer object
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint EBO;  //Element buffer object
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  //SET vertex attribute pointers and enable(input assembly)
  //Example if there is no known descriptor
  //GLuint attribLoc = glGetAttribLocation(coreProgram, "vertex_position");
  //glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
  //glEnableVertexAttribArray(attribLoc);

  //Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);
  //Color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
  glEnableVertexAttribArray(1);
  //Texture
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture));
  glEnableVertexAttribArray(2);
  //Normal
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(3);

  //bind vao 0(unbind enything)
  glBindVertexArray(0);

  //Texturing
  Texture texture0("Images/apple.png", GL_TEXTURE_2D, 0);
  Texture texture1("Images/flower.png", GL_TEXTURE_2D, 1);
  //Material
  Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.GetTextureUnit(), texture1.GetTextureUnit());

  //First Transforms
  glm::vec3 position(0.f);
  glm::vec3 rotation(0.f);
  glm::vec3 scale(1.f);

  glm::mat4 ModelMatrix(1.f);
  ModelMatrix = glm::translate(ModelMatrix, position);
  ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
  ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
  ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
  ModelMatrix = glm::scale(ModelMatrix, scale);

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
  coreProgram.SetMat4fv(ModelMatrix, "ModelMat");
  coreProgram.SetMat4fv(ViewMatrix, "ViewMat");
  coreProgram.SetMat4fv(ProjectionMatrix, "ProjectionMat");

  coreProgram.SetVec3f(lightPos0, "LightPos0");
  coreProgram.SetVec3f(camPosition, "CameraPos");

  //Main loop
  while (!glfwWindowShouldClose(window))
  {
    //update input
    glfwPollEvents();
    updateInput(window, position, rotation, scale);

    //update
    updateInput(window);
    //draw

    //clear
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Update uniforms
    coreProgram.Set1i(texture0.GetTextureUnit(), "texture0");
    coreProgram.Set1i(texture1.GetTextureUnit(), "texture1");
    material0.SendToShader(coreProgram);

    //Move/rotate
    //position.z -= 0.001f;
    //rotation.y += 0.5f;

    ModelMatrix = glm::mat4(1.f);
    ModelMatrix = glm::translate(ModelMatrix, position);
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
    ModelMatrix = glm::scale(ModelMatrix, scale);
    coreProgram.SetMat4fv(ModelMatrix, "ModelMat");

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

    //bind VBO
    glBindVertexArray(VAO);
    //draw
    //glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
    glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

    //Draw Mesh
    //testMesh.Render(&coreProgram);

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