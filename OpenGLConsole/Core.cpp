#include "stdafx.h"
#include "Core.h"
#include "Material.h"
#include "MeshNode.h"
#include "lights/LightNode.h"
#include "CoreDevice.h"
#include "lights/LightManager.h"

Core::Core(std::string title,
           const int width, const int height,
           int GLMajorVer, int GLMinorVer,
           bool resizable):
  _device(std::make_unique<CoreDevice>()),
  _window(std::make_unique<CoreWindow>(width, height, GLMajorVer, GLMinorVer)),
  _GLVerMajor(GLMajorVer),
  _GLVerMinor(GLMinorVer),
  _fov(0.f),
  _nearPlane(0.f),
  _farPlane(0.f),
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
  _camera(nullptr),
  _lightManager(std::make_shared<LightManager>())
{
  if (_window->InitWindow(title, resizable))
  {
    initGLEW();
    initOpenGlOptions();


    //Addition frame buffer test
    glGenFramebuffers(1, &_tmpFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _tmpFramebuffer);
    //
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &_tmpColorbuffer);
    glBindTexture(GL_TEXTURE_2D, _tmpColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tmpColorbuffer, 0);
    //
    glGenRenderbuffers(1, &_tmpRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, _tmpRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 768);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _tmpRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      std::cout << "TMP framebuffer init error" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(0);

    //Quad
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &_tmpQuadVAO);
    glGenBuffers(1, &_tmpQuadVBO);
    glBindVertexArray(_tmpQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, _tmpQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    //Shader
    _tmpShader = std::make_shared<Shader>(4, 5, "screen_quad.vert", "screen_quad.frag");
    //~Addition frame buffer test
  }
}

Core::~Core()
{
}


void Core::Update()
{
  updateDeltaTime();

  glfwPollEvents();
  updateKeyboardInput();
  updateMouseInput();

  _camera->UpdateInput(_deltaTime, -1, _mouseOffsetX, _mouseOffsetY);

  _device->ProcessAll();

  for (const auto& rootNode : _sceneNodes)
    rootNode->Update(_deltaTime);

  for (const auto& rootNode : _windshieldNodes)
    rootNode->Update(_deltaTime);
}

void Core::Render()
{
  //Second Pass prepare - render main pass to tmp framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, _tmpFramebuffer);

  //Original
  updateProjection(Projection_type::PERSPECTIVE);
  updateView();

    //clear (Move to startDraw func?)
  glClearColor(0.1f, 0.1f, 0.1f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  for (const auto& rootNode : _sceneNodes)
    rootNode->Render(_viewMat, _projectionMat, _camera, _lightManager);

  updateProjection(Projection_type::ORTHO);

  for (const auto& rootNode : _windshieldNodes)
    rootNode->Render(_viewMat, _projectionMat, _camera, _lightManager);

  //Second Pass
 // auto btn = std::make_shared<MeshNode>(std::make_shared<Quad>(), glm::vec3(100.f, 718.f, 0.f), glm::vec3(0.f), glm::vec3(200.f, 100.f, 1.f), _materials[2]);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  _tmpShader->Set1i(0, "screenTexture");
  _tmpShader->Use();

  glBindVertexArray(_tmpQuadVAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _tmpColorbuffer);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  //draw here a second quad

  glEnable(GL_DEPTH_TEST); //return depth test

  //End draw
  _window->SwapBuffers();
  glFlush();

  //Clear up all (Move to endDraw func?)
  glBindVertexArray(0);
  glUseProgram(0);
  glActiveTexture(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

int Core::GetWindiwShouldClose()
{
  return _window->GetWindowShouldClose();
}

void Core::SetWindowShouldClose()
{
  _window->SetWindowShouldClose(true);
}

//TODO Create event thread
void Core::RegisterKeyCallback(int key, std::function<void(void)> callback)
{
  _window->AddKeyCallback(key, callback);
}

void Core::UnregisterKeyCallback(int key)
{
  _window->RemoveKeyCallback(key);
}

void Core::SetCamera(std::shared_ptr<Camera> camera, float fov, float nearPlane, float farPlane)
{
  _camera = camera;
  _fov = fov;
  _nearPlane = nearPlane;
  _farPlane = farPlane;
}

void Core::AddLightSceneNode(std::shared_ptr<LightNodeBase> light, std::shared_ptr<CoreNode> parent /*= nullptr*/)
{
  _lightManager->RegisterLight(light->GetLightData());

  if (parent)
    parent->AddChild(light);
  else
    _sceneNodes.push_back(light);
}

void Core::AddMeshSceneNode(std::shared_ptr<MeshNode> mesh, std::shared_ptr<CoreNode> parent /*= nullptr*/)
{
  if (parent)
    parent->AddChild(mesh);
  else
    _sceneNodes.push_back(mesh);
}

void Core::AddWindshieldSceneNode(std::shared_ptr<MeshNode> mesh, std::shared_ptr<CoreNode> parent /*= nullptr*/)
{
  if (parent)
    parent->AddChild(mesh);
  else
    _windshieldNodes.push_back(mesh);
}

void Core::AddTextSceneNode(std::shared_ptr<TextNode> mesh, std::shared_ptr<CoreNode> parent /*= nullptr*/)
{
  if (parent)
    parent->AddChild(mesh);
  else
    _windshieldNodes.push_back(mesh);
}

void Core::AddCommand(CoreBaseCommand* command)
{
  _device->AddCommand(command);
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
  //glDepthFunc(GL_LESS); // Test of depth variant

  //glEnable(GL_STENCIL_TEST);
 // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
 // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
 
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //Input options
  _window->HideCoursor(true);
}

void Core::updateDeltaTime()
{
  _curTime = static_cast<float>(glfwGetTime());
  _deltaTime = _curTime - _lastTime;
  _lastTime = _curTime;
}

void Core::updateKeyboardInput()
{
  if (glfwGetKey(_window->Window(), GLFW_KEY_W) == GLFW_PRESS)
  {
    _camera->UpdateKeyboardInput(_deltaTime, Camera::FORWARD);
  }
  if (glfwGetKey(_window->Window(), GLFW_KEY_S) == GLFW_PRESS)
  {
    _camera->UpdateKeyboardInput(_deltaTime, Camera::BACK);
  }
  if (glfwGetKey(_window->Window(), GLFW_KEY_A) == GLFW_PRESS)
  {
    _camera->UpdateKeyboardInput(_deltaTime, Camera::LEFT);
  }
  if (glfwGetKey(_window->Window(), GLFW_KEY_D) == GLFW_PRESS)
  {
    _camera->UpdateKeyboardInput(_deltaTime, Camera::RIGHT);
  }
  if (glfwGetKey(_window->Window(), GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    _camera->UpdateKeyboardInput(_deltaTime, Camera::UP);
  }
  if (glfwGetKey(_window->Window(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    _camera->UpdateKeyboardInput(_deltaTime, Camera::DOWN);
  }
}

void Core::updateMouseInput()
{
  glfwGetCursorPos(_window->Window(), &_mouseX, &_mouseY);
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

void Core::updateProjection(Projection_type projection)
{
  //To handle resizing of the window
  //glfwGetFramebufferSize(_window, &_framebufferWidth, &_frameBufferHeight);
  auto frameBufferSize = _window->GetFramebufferSize();
  int framebufferWidth(frameBufferSize.first);
  int frameBufferHeight(frameBufferSize.second);

  if (projection == Projection_type::PERSPECTIVE)
  {
    _projectionMat = glm::perspective(
      glm::radians(_fov),
      static_cast<float>(framebufferWidth) / frameBufferHeight,
      _nearPlane,
      _farPlane);
  }
  else if (projection == Projection_type::ORTHO)
  {
    _projectionMat = glm::ortho(
      0.0f,
      static_cast<float>(framebufferWidth),
      0.f,
      static_cast<float>(frameBufferHeight),
      -1.f,
      1.f
      );
  }
}

void Core::updateView()
{
  _viewMat = _camera->GetViewMatrix();
}
