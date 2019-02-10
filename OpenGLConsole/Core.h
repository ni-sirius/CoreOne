#pragma once
#include <memory>

//Temp enums
enum Shader_enum {SHADER_CORE_PROGRAM};
enum Texture_enum {DIFFUSE_TEX_APPLE, SPECULAR_TEX_APPLE, DIFFUSE_TEX_FLOWER, SPECULAR_TEX_FLOWER};
enum Material_enum {STD_MAT};
enum Mesh_enum { MESH_QUAD };

class Shader;
class Texture;
class Material;
class Mesh;

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

  glm::vec3 _cameraPos;
  glm::vec3 _cameraFront;
  glm::vec3 _worldUp;
  float _fov;
  float _nearPlane;
  float _farPlane;

  //Graphic stuff
  std::vector< std::shared_ptr<Shader> > _shaders;
  std::vector< std::shared_ptr<Texture> > _textures;
  std::vector< std::shared_ptr<Material> > _materials;
  std::vector< std::shared_ptr<Mesh> > _meshes;
  std::vector< std::shared_ptr<glm::vec3> > _lights;

private:
  bool initGLFW();
  bool initWindow(std::string title, bool resizable);
  bool initGLEW();
  void initOpenGlOptions();

  void initMatrices();
  void initShaders();
  void initTextures();
  void initMaterials();
  void initMeshes();

  void initUniforms();

  void initLights();

  //Updaters
  void updateInput(GLFWwindow* window);
  void updateInput(GLFWwindow* window, Mesh& mesh);

  void updateUniforms();
};

