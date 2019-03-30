#pragma once
#include <memory>
#include <functional>
#include "LightNode.h"
#include "MeshNode.h"
#include "TextNode.h"
#include "CoreWindow.h"

//Temp enums
enum Shader_enum {SHADER_CORE_PROGRAM};
enum Texture_enum {DIFFUSE_TEX_APPLE, SPECULAR_TEX_APPLE, DIFFUSE_TEX_FLOWER, SPECULAR_TEX_FLOWER, CONT, CONT_SPEC};
enum Material_enum {STD_MAT, LIGHT_MAT};
enum Mesh_enum { MESH_QUAD };

enum class Projection_type {ORTHO, PERSPECTIVE};

class Shader;
class Texture;
class Material;
class Mesh;
class Camera;
class CoreDevice;
class CoreBaseCommand;

class Core
{
public:
  Core(std::string title,
       const int width, const int height,
       int GLMajorVer, int GLMinorVer,
       bool resizable);
  virtual ~Core();

  void Update();
  void Render();

  int GetWindiwShouldClose();
  void SetWindowShouldClose();

  void RegisterKeyCallback(int key, std::function<void(void)> callback);
  void UnregisterKeyCallback(int key);

  void SetCamera(std::shared_ptr<Camera> camera, float fov, float nearPlane, float farPlane);

  void AddLightSceneNode(std::shared_ptr<LightNode> light, std::shared_ptr<CoreNode> parent = nullptr);
  void AddMeshSceneNode(std::shared_ptr<MeshNode> mesh, std::shared_ptr<CoreNode> parent = nullptr);
  void AddWindshieldSceneNode(std::shared_ptr<MeshNode> mesh, std::shared_ptr<CoreNode> parent = nullptr);
  void AddTextSceneNode(std::shared_ptr<TextNode> mesh, std::shared_ptr<CoreNode> parent = nullptr);

  void AddCommand(CoreBaseCommand* command);

private:
  //Commands engine
  std::unique_ptr<CoreDevice> _device;

  //Window parameters
  std::unique_ptr<CoreWindow> _window;

  //OpenGL context
  const int _GLVerMajor;
  const int _GLVerMinor;

  //Matrices
  glm::mat4 _viewMat;
  glm::mat4 _projectionMat;

  //Camera
  float _fov;
  float _nearPlane;
  float _farPlane;
  std::shared_ptr<Camera> _camera;

  //Graphic stuff
  std::vector< std::shared_ptr<CoreNode> > _sceneNodes;
  std::vector< std::shared_ptr<PointLight> > _lightNodes;
  std::vector< std::shared_ptr<CoreNode> > _windshieldNodes;

  //delta time
  float _deltaTime;
  float _curTime;
  float _lastTime;

  //Mouse input
  double _lastMouseX;
  double _lastMouseY;
  double _mouseX;
  double _mouseY;
  double _mouseOffsetX;
  double _mouseOffsetY;
  bool _firstMouse;

private:
  bool initGLEW();
  void initOpenGlOptions();

  //Updaters
  void updateDeltaTime();

  void updateKeyboardInput();
  void updateMouseInput();

  void updateProjection(Projection_type projection);
  void updateView();
};

