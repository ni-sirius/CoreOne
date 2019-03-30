#include "stdafx.h"
#include "Render.h"

#include "Core.h"
#include <thread>
#include "CoreCommands.h"

std::vector< std::shared_ptr<Shader> > shaders;
std::vector< std::shared_ptr<Material> > materials;
std::vector< std::shared_ptr<Texture> > textures;

void initShaders(int GLVerMajor, int GLVerMinor)
{
  shaders.push_back(std::make_shared<Shader>(GLVerMajor, GLVerMinor, "vertex_core.glsl", "fragment_core.glsl"));
  shaders.push_back(std::make_shared<Shader>(GLVerMajor, GLVerMinor, "vertex_core.glsl", "fragment_light.glsl"));

  shaders.push_back(std::make_shared<Shader>(GLVerMajor, GLVerMinor, "windshield.vert", "windshield.frag"));

  shaders.push_back(std::make_shared<Shader>(GLVerMajor, GLVerMinor, "text.vert", "text.frag"));
}

void initMaterials()
{
  materials.push_back(
    std::make_shared<Material>(
      glm::vec3(0.1f), 32, 0, 1, shaders[0]));

  materials.push_back(
    std::make_shared<Material>(
      glm::vec3(1.0f), 32, 0, 1, shaders[1]));

  materials.push_back(
    std::make_shared<Material>(
      glm::vec3(1.0f), 32, 0, 1, shaders[2]));

  materials.push_back(
    std::make_shared<Material>(
      glm::vec3(1.0f), 32, 0, 1, shaders[3]));
}

void initTextures()
{
  textures.push_back(std::make_shared<Texture>("Images/apple.png", GL_TEXTURE_2D));
  textures.push_back(std::make_shared<Texture>("Images/apple_specular.png", GL_TEXTURE_2D));

  textures.push_back(std::make_shared<Texture>("Images/flower.png", GL_TEXTURE_2D));
  textures.push_back(std::make_shared<Texture>("Images/flower_specular.png", GL_TEXTURE_2D));

  textures.push_back(std::make_shared<Texture>("Images/container.png", GL_TEXTURE_2D));
  textures.push_back(std::make_shared<Texture>("Images/container_specular.png", GL_TEXTURE_2D));
}

class Foo
{
public:
  Foo(std::shared_ptr<TextNode> text, Core* core) {
    _text = text;
    _core = core;
  };

  void callback() {
    static float myscale(1.3f);
    myscale += 0.1;
    auto command = new SetTextScaleCommand(myscale, _text);
    _core->AddCommand(command);
  };

private:
  std::shared_ptr<TextNode> _text;
  Core* _core;
};

void Create(Core& core)
{
  initShaders(4, 5);
  initMaterials();
  initTextures();

  //Camera
  auto camera = std::make_shared<Camera>(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
  core.SetCamera(camera, 90.f, 0.1f, 1000.f);

  core.AddCommand(new SetCameraPositionCommand(glm::vec3(2.f, 0.f, 3.f), camera));

  //Lights
  auto lightNode = std::make_shared<LightNode>(glm::vec3(2.f, 1.5f, 1.f), glm::vec3(1.f));
  core.AddLightSceneNode(lightNode);

  auto lightMesh = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.5f), materials[1]);
  core.AddMeshSceneNode(lightMesh, lightNode);

  //Meshes
  auto flowerBox = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(-1.f, 1.f, 0.f), glm::vec3(0.f), glm::vec3(1.f), materials[0], textures[2], textures[3]);
  core.AddMeshSceneNode(flowerBox);

  auto container1 = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f), materials[0], textures[4], textures[5]);
  core.AddMeshSceneNode(container1);

  auto container2 = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f), materials[0], textures[4], textures[5]);
  core.AddMeshSceneNode(container2, container1);

  auto btn = std::make_shared<MeshNode>(std::make_shared<Quad>(), glm::vec3(100.f, 718.f, 0.f), glm::vec3(0.f), glm::vec3(200.f, 100.f, 1.f), materials[2]);
  core.AddWindshieldSceneNode(btn);

  auto txt = std::make_shared<TextNode>("Text Node", glm::vec3(25.f, 25.f, 0.f), 1.f, materials[3]);
  core.AddTextSceneNode(txt);

  auto txt1 = std::make_shared<TextNode>("Text Node", glm::vec3(25.f, 125.f, 0.f), 1.3f, materials[3]);
  core.AddTextSceneNode(txt1);

  
  //Var2
  auto qPressCall = [txt1, &core]() {
    static float myscale(1.3f);
    myscale += 0.1;
    auto command = new SetTextScaleCommand(myscale, txt1);
    core.AddCommand(command);
  };
  core.RegisterKeyCallback(GLFW_KEY_O, qPressCall);

  auto commandv = new SetMeshNodeVisibleCommand(false, btn);
  core.AddCommand(commandv);

  auto moveligthfunc = [&](std::shared_ptr<LightNode> light) {
    double oldTime(0);
    while (true)
    {
      auto time = glfwGetTime();
      if (time != oldTime)
        oldTime = time;
      else
        break;

      auto newPos = glm::vec3(glm::sin(time) * 2, 1.f, glm::cos(time) * 2);

      auto command = new SetLightNodePositionCommand(newPos, light);
      core.AddCommand(command);
    }
  };

  auto changeTextFunc = [&](std::shared_ptr<TextNode> text) {
    while (true)
    {

    }
  };

  auto tstThread = std::thread(moveligthfunc, lightNode);
  tstThread.detach();//TODO Create normal class
}

void RenderLoop()
{
  Core coreObject("OpenGL Test App", 1024, 768, 4, 5, false);

  Create(coreObject);

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