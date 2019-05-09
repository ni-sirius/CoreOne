#pragma once
#include <thread>
#include "Core.h"
#include "CoreCommands.h"


class Renderer final
{
public:
  Renderer();
  ~Renderer();

  void Run();

private:
  void initShaders();
  void initMaterials();
  void initTextures();

  void initCamera();
  void initSceneObjects();
  void initLights();
  void initKeyCallbacks();

  void workerThread();
  void render();

private:
  const std::pair<int, int> _glVersion{ 4,5 };

  Core _core;

  std::thread _userThread;
  bool _userThreadRunning;

  std::vector< std::shared_ptr<Shader> > _shaders;
  std::vector< std::shared_ptr<Material> > _materials;
  std::vector< std::shared_ptr<Texture> > _textures;

  std::vector<std::shared_ptr<LightNode> > _lights;
  std::vector<std::shared_ptr<MeshNode> > _meshes;
  std::vector<std::shared_ptr<CoreNode> > _windshields;
};
