#pragma once
#include <thread>
#include "Core.h"
#include "CoreWindow.h"
#include "CoreCommands.h"

using namespace coreone;

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

  void runExamples();
  void stencilExample();
  void textsExample();
  void blendingExample();

private:
  const std::pair<int, int> _glVersion{ 4,5 };
  corewindow::CoreWindow _oglWindow;

  Core _core;

  std::thread _userThread;
  bool _userThreadRunning;

  std::vector<std::shared_ptr<LightNodeBase> > _lights;
  std::vector<std::shared_ptr<MeshNode> > _meshes;
  std::vector<std::shared_ptr<CoreNode> > _windshields;
};
