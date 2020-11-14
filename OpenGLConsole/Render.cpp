#include "stdafx.h"
#include "Render.h"

#include "Core.h"
#include <thread>
#include "CoreCommands.h"

#include "MeshNode.h"
#include "TextNode.h"

#include "lights/LightNode.h"
#include "lights/PointLightNode.h"
#include "lights/SpotLightNode.h"
#include "lights/DirectionLightNode.h"

#include "states/CoreState.h"

Renderer::Renderer():
  _core(),
  _userThreadRunning(true)
{
  _oglWindow.Init(1024, 768, 4, 5, "CoreOne Test App", true);
  _oglWindow.HideCoursor(false);
  _core.SetWindow(&_oglWindow);

  initShaders();
  initMaterials();
  initTextures();

  initCamera();
  initSceneObjects();
  initLights();
  initKeyCallbacks();
}

Renderer::~Renderer()
{
  _userThreadRunning = false;
  _userThread.join();
}

void Renderer::Run()
{
  _userThread = std::thread(&Renderer::workerThread, this);

  render();
}

void Renderer::initShaders()
{
  _shaders.push_back(std::make_shared<Shader>(_glVersion.first, _glVersion.second, "vertex_core.glsl", "fragment_core.glsl"));
  _shaders.push_back(std::make_shared<Shader>(_glVersion.first, _glVersion.second, "vertex_core.glsl", "fragment_light.glsl"));

  _shaders.push_back(std::make_shared<Shader>(_glVersion.first, _glVersion.second, "windshield.vert", "windshield.frag"));

  _shaders.push_back(std::make_shared<Shader>(_glVersion.first, _glVersion.second, "text.vert", "text.frag"));
}

void Renderer::initMaterials()
{
  _materials.push_back(
    std::make_shared<Material>(
      glm::vec3(0.1f), 32, 0, 1, _shaders[0]));

  _materials.push_back(
    std::make_shared<Material>(
      glm::vec3(1.0f), 32, 0, 1, _shaders[1]));

  _materials.push_back(
    std::make_shared<Material>(
      glm::vec3(1.0f), 32, 0, 1, _shaders[2]));

  _materials.push_back(
    std::make_shared<Material>(
      glm::vec3(1.0f), 32, 0, 1, _shaders[3]));
}

void Renderer::initTextures()
{
  _textures.push_back(std::make_shared<graphics::Texture>("Images/apple.png", GL_TEXTURE_2D));
  _textures.push_back(std::make_shared<graphics::Texture>("Images/apple_specular.png", GL_TEXTURE_2D));

  _textures.push_back(std::make_shared<graphics::Texture>("Images/flower.png", GL_TEXTURE_2D));
  _textures.push_back(std::make_shared<graphics::Texture>("Images/flower_specular.png", GL_TEXTURE_2D));

  _textures.push_back(std::make_shared<graphics::Texture>("Images/container.png", GL_TEXTURE_2D));
  _textures.push_back(std::make_shared<graphics::Texture>("Images/container_specular.png", GL_TEXTURE_2D));

  _textures.push_back(std::make_shared<graphics::Texture>("Images/window.png", GL_TEXTURE_2D));
  _textures.push_back(std::make_shared<graphics::Texture>("Images/grass.png", GL_TEXTURE_2D));
}

void Renderer::initCamera()
{
  auto camera = std::make_shared<Camera>(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
  _core.SetCamera(camera, 90.f, 0.1f, 1000.f);
}

void Renderer::initSceneObjects()
{
  auto floor = std::make_shared<MeshNode>(
    std::make_shared<Quad>(),
    glm::vec3(0.f, -0.5f, 0.f),
    glm::vec3(-90.f, 0.f, 0.f),
    glm::vec3(10.f, 10.f, 1.f),
    _materials[0], _textures[2]
    );
  _core.AddMeshSceneNode(floor);
  _meshes.push_back(floor);

  auto container1 = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(2.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f), _materials[0], _textures[4], _textures[5]);
  _core.AddMeshSceneNode(container1);
  _meshes.push_back(container1);

  auto container2 = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f), glm::vec3(1.5f), _materials[0], _textures[4], _textures[5]);
  _core.AddMeshSceneNode(container2, container1);
  _meshes.push_back(container2);

  auto container3 = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(-1.f, 0.f, -1.f), glm::vec3(0.f), glm::vec3(1.f), _materials[0], _textures[4], _textures[5]);
  _core.AddMeshSceneNode(container3);
  _meshes.push_back(container3);

  auto btn = std::make_shared<MeshNode>(std::make_shared<Quad>(), glm::vec3(100.f, 718.f, 0.f), glm::vec3(0.f), glm::vec3(200.f, 100.f, 1.f), _materials[2]);
  _core.AddWindshieldSceneNode(btn);
  _windshields.push_back(btn);

  auto flowerBox = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f), _materials[0], _textures[2], _textures[3]);
  auto state = flowerBox->GetOrCreateCoreState();
  state->SetType(CoreState::OVERRIDE);
  state->SetPreExecCallback([]() {glDisable(GL_CULL_FACE); });
  state->SetPostExecCallback([]() {glEnable(GL_CULL_FACE); });
  _core.AddMeshSceneNode(flowerBox);
  _meshes.push_back(flowerBox);

  runExamples();
}

void Renderer::initLights()
{
  //Lights
  auto lightNode = std::make_shared<PointLightNode>();
  _core.AddLightSceneNode(lightNode);
  _lights.push_back(lightNode);

  auto lightNode2 = std::make_shared<PointLightNode>();
  lightNode2->SetPosition(glm::vec3(0.f, 0.f, -1.f));
  _core.AddLightSceneNode(lightNode2);
  _lights.push_back(lightNode2);

  auto lightNode3 = std::make_shared<SpotLightNode>();
  _core.AddLightSceneNode(lightNode3);
  _lights.push_back(lightNode3);

  auto lightNode4 = std::make_shared<DirectionLightNode>();
  _core.AddLightSceneNode(lightNode4);
  _lights.push_back(lightNode4);

  auto lightMesh = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.5f), _materials[1]);
  _core.AddMeshSceneNode(lightMesh, lightNode);
}

void Renderer::initKeyCallbacks()
{
  auto escCallback = [this]() {
    _oglWindow.SetWindowShouldClose();
    };
  _oglWindow.AddKeyCallback(GLFW_KEY_ESCAPE, escCallback);

  auto textNode = std::static_pointer_cast<TextNode>(_windshields[1]);
  auto oPressCall = [this, textNode]() {
    static float myscale(1.3f);
    myscale += 0.1f;
    auto command = new SetTextScaleCommand(myscale, textNode);
    _core.AddCommand(command);
  };
  _oglWindow.AddKeyCallback(GLFW_KEY_O, oPressCall);
}

void Renderer::workerThread()
{
  double oldTime(0);
  while (_userThreadRunning)
  {
    auto time = glfwGetTime();
    if (time != oldTime)
      oldTime = time;
    else
      break;
    
    auto newPos = glm::vec3(glm::sin(time) * 2, 1.f, glm::cos(time) * 2);
    
    auto command = new SetLightNodePositionCommand(newPos, std::static_pointer_cast<PointLightNode>(_lights[0]));
    _core.AddCommand(command);
  }
}

void Renderer::render()
{
  while (!_oglWindow.GetWindowShouldClose())
  {
    _core.Update();
    _core.Render();
  }
}

void Renderer::runExamples()
{
 //stencilExample();
 textsExample();
 blendingExample();
}

void Renderer::stencilExample()
{
  auto flowerBox = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(5.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f), _materials[0], _textures[2], _textures[3]);
  auto state = flowerBox->GetOrCreateCoreState();
  state->SetType(CoreState::OVERRIDE);
  state->SetPreExecCallback([]() {
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    });
  state->SetPostExecCallback([]() {
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    });
  _core.AddMeshSceneNode(flowerBox);
  _meshes.push_back(flowerBox);
}

void Renderer::textsExample()
{
  auto txt = std::make_shared<TextNode>("Text Node", glm::vec3(25.f, 25.f, 0.f), 1.f, _materials[3]);
  _core.AddTextSceneNode(txt);
  _windshields.push_back(txt);

  auto txt1 = std::make_shared<TextNode>("Text Node", glm::vec3(25.f, 125.f, 0.f), 1.3f, _materials[3]);
  _core.AddTextSceneNode(txt1);
  _windshields.push_back(txt1);
}

void Renderer::blendingExample()
{
  auto gr1 = std::make_shared<MeshNode>(std::make_shared<Quad>(), glm::vec3(0.1f, 0.f, 1.5f), glm::vec3(0.f), glm::vec3(1.f), _materials[0], _textures[6]);
  auto stategr1 = gr1->GetOrCreateCoreState();
  stategr1->SetPreExecCallback([]() {glDisable(GL_CULL_FACE); });
  stategr1->SetPostExecCallback([]() {glEnable(GL_CULL_FACE); });
  _core.AddMeshSceneNode(gr1);
  _meshes.push_back(gr1);

  auto gr11 = std::make_shared<MeshNode>(std::make_shared<Quad>(), glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f), glm::vec3(1.f), _materials[0], _textures[6]);
  auto stategr11 = gr11->GetOrCreateCoreState();
  stategr11->SetPreExecCallback([]() {glDisable(GL_CULL_FACE); });
  stategr11->SetPostExecCallback([]() {glEnable(GL_CULL_FACE); });
  _core.AddMeshSceneNode(gr11);
  _meshes.push_back(gr11);
}
