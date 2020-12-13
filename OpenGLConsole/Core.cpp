#include "stdafx.h"
#include "Core.h"
#include "CoreWindow.h"
#include "MeshNode.h"
#include "lights/LightNode.h"
#include "TextNode.h"
#include "CoreDevice.h"
#include "lights/LightManager.h"
#include <input/ResourceManager.h>
#include <graphics/Shader.h>

using namespace coreone;

Core::Core():
  _device(std::make_unique<CoreDevice>()),
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
}

Core::~Core()
{
  glDeleteFramebuffers(1, &_tmpFramebuffer);
}

void Core::SetWindow(corewindow::CoreWindow* window)
{
  if (window)
  {
    _window = window;

    initGLEW();
    initOpenGlOptions();

    //TODO Create Default camera


    //Addition frame buffer test
    glGenFramebuffers(1, &_tmpFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _tmpFramebuffer);
    //
    _tmpClrBuffer.CreateTexture(GL_TEXTURE_2D, Texture::COLOR, 1024, 768);
    _tmpClrBuffer.Bind(0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tmpClrBuffer.GetID(), 0);
    _tmpClrBuffer.Unbind();
    //TMP
    _tmpDSTex.CreateTexture(GL_TEXTURE_2D, Texture::DEPTH_STENCIL, 1024, 768);
    _tmpDSTex.Bind(0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _tmpDSTex.GetID(), 0);
    _tmpDSTex.Unbind();
    //TMP
    //
    _tmpStenDepthRB.CreateRenderBuffer(RenderBuffer::DEPTH_STENCIL, 1024, 768);
    _tmpStenDepthRB.Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _tmpStenDepthRB.GetID());
    _tmpStenDepthRB.Unbind();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      std::cout << "TMP framebuffer init error" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //_tmpQuad = std::make_shared<MeshNode>(std::make_shared<Quad>(), glm::vec3(0.1f, 0.f, 1.5f), glm::vec3(0.f), glm::vec3(1.f), _materials[0], _textures[6]);

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
    _tmpShader = ResourceManager::Instance().LoadShaderProgram("screen_quad", 4, 5, "screen_quad.vert", "screen_quad.frag");
    //~Addition frame buffer test
  }
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
  if (_window == nullptr || !_window->Initialized())
  {
    return;
  }

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
  _tmpShader->SetUniform("screenTexture", 0);
  _tmpShader->Use();

  glBindVertexArray(_tmpQuadVAO);

  _tmpClrBuffer.Bind(0);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  _tmpClrBuffer.Unbind();

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
}

void Core::updateDeltaTime()
{
  _curTime = static_cast<float>(glfwGetTime());
  _deltaTime = _curTime - _lastTime;
  _lastTime = _curTime;
}

void Core::updateKeyboardInput()
{
  if (_window == nullptr || !_window->Initialized())
  {
    return;
  }

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
  if (_window == nullptr || !_window->Initialized())
  {
    return;
  }

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
  if (_window == nullptr || !_window->Initialized())
  {
    return;
  }

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
    //float ratio = atan(glm::radians(_fov / 2.f)) * 2.f;
    //float sizex = ratio * 1;
    //float sizey = ratio * static_cast<float>(framebufferWidth) / frameBufferHeight;
    //auto distance = glm::length(mTarget - mEye);
    //glm::mat4 orthProject = glm::ortho(-sizex, sizex, -sizey, sizey, 0.0f, 2.0f*distance);

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
