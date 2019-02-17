#include "stdafx.h"
#include "Core.h"
#include "Material.h"
#include "Mesh.h"



Core::Core(std::string title,
           const int width, const int height,
           int GLMajorVer, int GLMinorVer,
           bool resizable):
  _window(nullptr),
  _windowWidth(width),
  _windowHeight(height),
  _framebufferWidth(0),
  _frameBufferHeight(0),
  _GLVerMajor(GLMajorVer),
  _GLVerMinor(GLMinorVer),
  _fov(90.f),
  _nearPlane(0.1f),
  _farPlane(1000.f),
  _projectionMat(1.f),
  _viewMat(1.f),
  _deltaTime(0.f),
  _curTime(0.f),
  _lastTime(0.f),
  _lastMouseX(0.0),
  _lastMouseY(0.0),
  _mouseX(0.0),
  _mouseY(0.0),
  _mouseOffsetX(0.0),
  _mouseOffsetY(0.0),
  _firstMouse(true),
  _camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f))
{
  if (initGLFW())
    if (initWindow(title, resizable))
    {
      initGLEW();
      initOpenGlOptions();
      initMatrices();
      initShaders();
      initTextures();
      initMaterials();
      initMeshes();
      initLights();
      initUniforms();
    }
}

Core::~Core()
{
  glfwDestroyWindow(_window);
  glfwTerminate();
}


void Core::Update()
{
  updateDeltaTime();

  glfwPollEvents();
  updateKeyboardInput();
  updateMouseInput();

  _camera.UpdateInput(_deltaTime, -1, _mouseOffsetX, _mouseOffsetY);
}

void Core::Render()
{
  //update move
  //updateInput(_window, *_meshes[MESH_QUAD]);
  //update Exit
  //updateInput(_window);

  //clear
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  //Uniforms
  updateUniforms();
  
  //Update uniforms
  _materials[Material_enum::STD_MAT]->SendToShader(*_shaders[SHADER_CORE_PROGRAM]);
  
  //use a program
  _shaders[SHADER_CORE_PROGRAM]->Use();
  //Activate texture
  _textures[DIFFUSE_TEX_FLOWER]->Bind(0);
  _textures[SPECULAR_TEX_FLOWER]->Bind(1);
  //Draw Mesh
  _meshes[MESH_QUAD]->Render(_shaders[SHADER_CORE_PROGRAM].get());

  //end draw
  glfwSwapBuffers(_window);
  glFlush();

  //free
  _shaders[SHADER_CORE_PROGRAM]->Unuse();
}

int Core::GetWindiwShouldClose()
{
  return glfwWindowShouldClose(_window);
}

void Core::SetWindowShouldClose()
{
  glfwSetWindowShouldClose(_window, GLFW_TRUE);
}

bool Core::initGLFW()
{
  if (glfwInit() == GLFW_FALSE)
  {
    std::cout << "CORE::GLFW::ERROR Init failed" << std::endl;
    glfwTerminate();

    return false;
  }

  return true;
}

bool Core::initWindow(std::string title, bool resizable)
{
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _GLVerMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _GLVerMinor);
  glfwWindowHint(GLFW_RESIZABLE, resizable);

  _window = glfwCreateWindow(_windowWidth, _windowHeight, title.c_str(), nullptr, nullptr);
  if (!_window)
  {
    std::cout << "WINDOW::ERROR Init failed" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwGetFramebufferSize(_window, &_framebufferWidth, &_frameBufferHeight); //for prrojection matrix

  auto framebufferResizeCallback = [](GLFWwindow* window, int frameBufWidth, int frameBufHeight) {
    glViewport(0, 0, frameBufWidth, frameBufHeight);
  };
  glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
  //glViewport(0, 0, framebufferWidth, frameBufferHeight); if static size

  glfwMakeContextCurrent(_window);

  return true;
}

bool Core::initGLEW()
{
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "ERROR::CORE GLEW Init Failed" << std::endl;
    glfwTerminate();

    return false;
  }

  return true;
}

void Core::initOpenGlOptions()
{
  glEnable(GL_DEPTH_TEST);

  //TODO learn culling and uncomment
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //Input options
  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Core::initMatrices()
{
  _viewMat = _camera.GetViewMatrix();

  _projectionMat = glm::perspective(
    glm::radians(_fov),
    static_cast<float>(_framebufferWidth) / _frameBufferHeight,
    _nearPlane,
    _farPlane);
}

void Core::initShaders()
{
  _shaders.push_back(std::make_shared<Shader>(_GLVerMajor, _GLVerMinor, "vertex_core.glsl", "fragment_core.glsl"));
}

void Core::initTextures()
{
  _textures.push_back(std::make_shared<Texture>("Images/apple.png", GL_TEXTURE_2D));
  _textures.push_back(std::make_shared<Texture>("Images/apple_specular.png", GL_TEXTURE_2D));

  _textures.push_back(std::make_shared<Texture>("Images/flower.png", GL_TEXTURE_2D));
  _textures.push_back(std::make_shared<Texture>("Images/flower_specular.png", GL_TEXTURE_2D));
}

void Core::initMaterials()
{
  _materials.push_back(
    std::make_shared<Material>(
      glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 0, 1));
}

void Core::initMeshes()
{
  _meshes.push_back(std::make_shared<Mesh>(&Pyramid(), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(2.f)));
}

void Core::initUniforms()
{
  _shaders[Shader_enum::SHADER_CORE_PROGRAM]->SetMat4fv(_viewMat, "ViewMat");
  _shaders[Shader_enum::SHADER_CORE_PROGRAM]->SetMat4fv(_projectionMat, "ProjectionMat");

  _shaders[Shader_enum::SHADER_CORE_PROGRAM]->SetVec3f(*_lights[0], "LightPos0");
}

void Core::initLights()
{
  _lights.push_back(std::make_shared<glm::vec3>(0.f, 1.5f, 1.5f));
}

void Core::updateDeltaTime()
{
  _curTime = static_cast<float>(glfwGetTime());
  _deltaTime = _curTime - _lastTime;
  _lastTime = _curTime;
}

void Core::updateInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void Core::updateInput(GLFWwindow* window, Mesh& mesh)
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

void Core::updateKeyboardInput()
{
  if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(_window, GLFW_TRUE);
  }

  if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
  {
    _camera.UpdateKeyboardInput(_deltaTime, Camera::FORWARD);
  }
  if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
  {
    _camera.UpdateKeyboardInput(_deltaTime, Camera::BACK);
  }
  if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
  {
    _camera.UpdateKeyboardInput(_deltaTime, Camera::LEFT);
  }
  if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
  {
    _camera.UpdateKeyboardInput(_deltaTime, Camera::RIGHT);
  }
  if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    _camera.UpdateKeyboardInput(_deltaTime, Camera::UP);
  }
  if (glfwGetKey(_window, GLFW_KEY_C) == GLFW_PRESS)
  {
    _camera.UpdateKeyboardInput(_deltaTime, Camera::DOWN);
  }
}

void Core::updateMouseInput()
{
  glfwGetCursorPos(_window, &_mouseX, &_mouseY);
  static bool firstFlag(true);
  if (firstFlag)
  {
    _lastMouseX = _mouseX;
    _lastMouseY = _mouseY;
    firstFlag = false;
  }

  _mouseOffsetX = _mouseX - _lastMouseX;
  _mouseOffsetY = _lastMouseY - _mouseY;

  _lastMouseX = _mouseX;
  _lastMouseY = _mouseY;
}

void Core::updateUniforms()
{
  _viewMat = _camera.GetViewMatrix();//glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _worldUp);
  _shaders[SHADER_CORE_PROGRAM]->SetMat4fv(_viewMat, "ViewMat");

  _shaders[Shader_enum::SHADER_CORE_PROGRAM]->SetVec3f(_camera.GetCameraPosition()/*_cameraPos*/, "CameraPos");

  //To handle resizing of the window
  glfwGetFramebufferSize(_window, &_framebufferWidth, &_frameBufferHeight);
  _projectionMat = glm::perspective(
    glm::radians(_fov),
    static_cast<float>(_framebufferWidth) / _frameBufferHeight,
    _nearPlane,
    _farPlane);
  _shaders[SHADER_CORE_PROGRAM]->SetMat4fv(_projectionMat, "ProjectionMat");
}
