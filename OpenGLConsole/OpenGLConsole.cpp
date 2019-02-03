// OpenGLConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Vertex
{
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 texture;
  glm::vec3 normal;
};

Vertex vertices[] = {
  //position                     //color                    //uvcoords               //normals                 
  glm::vec3(-0.5f, 0.5f, 0.0f),  glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f),      glm::vec3(0.f, 0.f, -1.f),
  glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f),      glm::vec3(0.f, 0.f, -1.f),
  glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),      glm::vec3(0.f, 0.f, -1.f),
  glm::vec3(0.5f, 0.5f, 0.0f),   glm::vec3(1.f, 1.f, 0.f), glm::vec2(1.f, 1.f),      glm::vec3(0.f, 0.f, -1.f)
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
    position.z += 0.01f;
  }
  else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    position.z -= 0.01f;
  }
  else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    position.x += 0.01f;
  }
  else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    position.x -= 0.01f;
  }
  else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    rotation.y -= 1.f;
  }
  else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    rotation.y += 1.f;
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

bool loadShaders(GLuint& program)
{
  bool loadSuccedeed(true);
  char infoLog[512];
  GLint success;

  std::string temp = "";
  std::string src = "";

  std::ifstream in_file;

  //Vertex
  in_file.open("vertex_core.glsl");
  if (in_file.is_open())
  {
    while (std::getline(in_file, temp))
      src += temp + "\n";
  }
  else
  {
    std::cout << "ERROR::LoadShaders could not open vertex file" << std::endl;
    loadSuccedeed = false;
  }
  in_file.close();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const GLchar* vertScr = src.c_str();
  glShaderSource(vertexShader, 1, &vertScr, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::LoadShaders could not compile vertex file" << std::endl;
    std::cout << infoLog << std::endl;
    loadSuccedeed = false;
  }

  //Fragment
  temp = "";
  src = "";
  in_file.open("fragment_core.glsl");
  if (in_file.is_open())
  {
    while (std::getline(in_file, temp))
      src += temp + "\n";
  }
  else
  {
    std::cout << "ERROR::LoadShaders could not open fragment file" << std::endl;
    loadSuccedeed = false;
  }
  in_file.close();

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const GLchar* fragScr = src.c_str();
  glShaderSource(fragmentShader, 1, &fragScr, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::LoadShaders could not compile fragment file" << std::endl;
    std::cout << infoLog << std::endl;
    loadSuccedeed = false;
  }

  //Program
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);

  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "ERROR::Could not link shader program" << std::endl;
    std::cout << infoLog << std::endl;
    loadSuccedeed = false;
  }

  //End
  glUseProgram(0);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return loadSuccedeed;
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
    std::cout << "Error::Texture on path "<< path <<" load failed" << std::endl;
  }

  glActiveTexture(0);
  glBindTexture(GL_TEXTURE_2D, 0); // set text to 0, because if all goes wrong there is random data. insted noeow
  SOIL_free_image_data(image);

  return texture;
}

int main()
{
  //glfwInit();

  //glewExperimental = GL_TRUE;
  //if (glewInit() != GLEW_OK)
  //  std::cout << "bad" << std::endl;
  //else
  //  std::cout << "good" << std::endl;

  //Init GLFW
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

  glfwGetFramebufferSize(window, &framebufferWidth, &frameBufferHeight);

  auto framebufferResizeCallback = [](GLFWwindow* window, int frameBufWidth, int frameBufHeight){
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
  GLuint coreProgram;
  if (!loadShaders(coreProgram))
    glfwTerminate();

  //MODEL

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
  GLuint texture0 = createTexture("Images/apple.png");
  GLuint texture1 = createTexture("Images/flower.png");

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
  glUseProgram(coreProgram);

  glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ModelMat"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
  glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ViewMat"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
  glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ProjectionMat"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

  glUniform3fv(glGetUniformLocation(coreProgram, "LightPos0"), 1, glm::value_ptr(lightPos0));
  glUniform3fv(glGetUniformLocation(coreProgram, "CameraPos"), 1, glm::value_ptr(camPosition));

  glUseProgram(0);

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
    //use a program
    glUseProgram(coreProgram);

    //Update uniforms
    glUniform1i(glGetUniformLocation(coreProgram, "texture0"), 0);
    glUniform1i(glGetUniformLocation(coreProgram, "texture1"), 1);

    //Move/rotate
    //position.z -= 0.001f;
    //rotation.y += 0.5f;

    ModelMatrix = glm::mat4(1.f);
    ModelMatrix = glm::translate(ModelMatrix, position);
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
    ModelMatrix = glm::scale(ModelMatrix, scale);
    glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ModelMat"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

    glfwGetFramebufferSize(window, &framebufferWidth, &frameBufferHeight);
    ProjectionMatrix = glm::perspective(
      glm::radians(fov),
      static_cast<float>(framebufferWidth) / frameBufferHeight,
      nearPlane,
      farPlane);
    glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ProjectionMat"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

    //Activate texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);


    //bind VBO
    glBindVertexArray(VAO);
    //draw
    //glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
    glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

    //end draw
    glfwSwapBuffers(window);
    glFlush();


    //free
    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  //Ending
  glfwDestroyWindow(window);
  glfwTerminate();

  glDeleteProgram(coreProgram);
  //system("pause");

  return 0;
}

