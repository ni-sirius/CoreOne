#pragma once
#include <memory>
#include <functional>


//Temp enums
enum Shader_enum {SHADER_CORE_PROGRAM};
enum Texture_enum {DIFFUSE_TEX_APPLE, SPECULAR_TEX_APPLE, DIFFUSE_TEX_FLOWER, SPECULAR_TEX_FLOWER, CONT, CONT_SPEC};
enum Material_enum {STD_MAT, LIGHT_MAT};
enum Mesh_enum { MESH_QUAD };

enum class Projection_type {ORTHO, PERSPECTIVE};

class Shader;
class Material;
class Mesh;
class Camera;
class CoreDevice;
class CoreBaseCommand;
class LightManager;
class LightNodeBase;
class MeshNode;
class TextNode;
class CoreNode;

namespace coreone
{
  namespace corewindow
  {
    class CoreWindow;
  }

  class Core
  {
  public:
    Core();
    virtual ~Core();

    void SetWindow(corewindow::CoreWindow* window);

    void Update();
    void Render();

    void SetCamera(std::shared_ptr<Camera> camera, float fov, float nearPlane, float farPlane);

    void AddLightSceneNode(std::shared_ptr<LightNodeBase> light, std::shared_ptr<CoreNode> parent = nullptr);
    void AddMeshSceneNode(std::shared_ptr<MeshNode> mesh, std::shared_ptr<CoreNode> parent = nullptr);
    void AddWindshieldSceneNode(std::shared_ptr<MeshNode> mesh, std::shared_ptr<CoreNode> parent = nullptr);
    void AddTextSceneNode(std::shared_ptr<TextNode> mesh, std::shared_ptr<CoreNode> parent = nullptr);

    void AddCommand(CoreBaseCommand* command);

  private:
    //Commands engine
    std::unique_ptr<CoreDevice> _device;

    //Window parameters
    corewindow::CoreWindow* _window = nullptr;

    //Matrices //DEPRECATED
    glm::mat4 _viewMat;
    glm::mat4 _projectionMat;

    //Camera //DEPRECATED
    float _fov;
    float _nearPlane;
    float _farPlane;
    std::shared_ptr<Camera> _camera;

    //Graphic stuff
    std::vector< std::shared_ptr<CoreNode> > _sceneNodes;
    std::vector< std::shared_ptr<CoreNode> > _windshieldNodes;
    std::shared_ptr<LightManager> _lightManager;

    //delta time
    float _deltaTime;
    float _curTime;
    float _lastTime;

    //Mouse input //DEPRECATED
    double _lastMouseX;
    double _lastMouseY;
    double _mouseX;
    double _mouseY;
    double _mouseOffsetX;
    double _mouseOffsetY;
    bool _firstMouse;

    //DEPRECATED
    unsigned int _tmpFramebuffer;
    graphics::Texture _tmpClrBuffer;
    graphics::Texture _tmpDSTex;
    unsigned int _tmpRBO;
    unsigned int _tmpQuadVAO;
    unsigned int _tmpQuadVBO;
    std::shared_ptr<Shader> _tmpShader;

  private:
    bool initGLEW();
    void initOpenGlOptions();

    //Updaters
    void updateDeltaTime();

    void updateKeyboardInput();//DEPRECATED
    void updateMouseInput();//DEPRECATED

    void updateProjection(Projection_type projection);//DEPRECATED
    void updateView();//DEPRECATED
  };
} // namespace coreone
