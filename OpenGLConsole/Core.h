#pragma once
#include <memory>
#include "LightNode.h"
#include "MeshNode.h"

//Temp enums
enum Shader_enum {SHADER_CORE_PROGRAM};
enum Texture_enum {DIFFUSE_TEX_APPLE, SPECULAR_TEX_APPLE, DIFFUSE_TEX_FLOWER, SPECULAR_TEX_FLOWER, CONT, CONT_SPEC};
enum Material_enum {STD_MAT, LIGHT_MAT};
enum Mesh_enum { MESH_QUAD };

class Shader;
class Texture;
class Material;
class Mesh;
class Camera;

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

  void AddLightSceneNode(LightNode* light, CoreNode* parent = nullptr);
  void AddMeshSceneNode(MeshNode* mesh, CoreNode* parent = nullptr);

private:
  //Window parameters
  GLFWwindow* _window;
  const int _windowWidth;
  const int _windowHeight;
  int _framebufferWidth;
  int _frameBufferHeight;

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
  Camera _camera;

  //Graphic stuff
  std::shared_ptr<CoreNode> _rootNode;
  std::vector< std::shared_ptr<LightNode> > _lightNodes;

  //Resource Manager part TODO
  std::vector< std::shared_ptr<Shader> > _shaders;
  std::vector< std::shared_ptr<Texture> > _textures;
  std::vector< std::shared_ptr<Material> > _materials;

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
  bool initGLFW();
  bool initWindow(std::string title, bool resizable);
  bool initGLEW();
  void initOpenGlOptions();

  void initMatrices();
  void initShaders();

  void initUniforms();

  //Updaters
  void updateDeltaTime();

  void updateKeyboardInput();
  void updateMouseInput();

  //Object move(unused)
  void updateInput(GLFWwindow* window);
  void updateInput(GLFWwindow* window, Mesh& mesh);

  void updateUniforms();
};

