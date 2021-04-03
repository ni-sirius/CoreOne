#pragma once
#include "CoreNode.h"

struct Vertex;
class CoreState;

namespace coreone
{
  class Primitive;

  class MeshNode : public CoreNode
  {
  public:
    MeshNode(std::shared_ptr<Primitive> primitive,
      std::shared_ptr<graphics::Material> material = nullptr);
    virtual ~MeshNode();

    virtual void Update(const float& deltaTime, glm::mat4 modelMatrix = glm::mat4(1.f)) override;
    virtual void Render(glm::mat4 viewMat, glm::mat4 projectionMat,
      std::shared_ptr<Camera> camera,
      std::shared_ptr<LightManager> lightManager) override;

    CoreState* const GetOrCreateCoreState();
    void SetCoreState(std::shared_ptr<CoreState> state);

    inline void SetVisible(bool visible) { _visible = visible; };

    inline void SetPosition(const glm::vec3 position) { _position = position; };
    inline void SetRotation(const glm::vec3 rotation) { _rotation = rotation; };
    inline void SetScale(const glm::vec3 scale) { _scale = scale; };

    inline void Move(const glm::vec3 position) { _position += position; };
    inline void Rotate(const glm::vec3 rotation) { _rotation += rotation; };
    inline void Upscale(const glm::vec3 upscale) { _scale += upscale; };

    inline void SetMaterial(std::shared_ptr<graphics::Material> material) { _material = material; }

  protected:
    void updateModelMatrix();

  protected:
    bool _visible;

    glm::vec3 _position = glm::vec3(0.f);
    glm::vec3 _rotation = glm::vec3(0.f);
    glm::vec3 _scale = glm::vec3(1.f);

    std::shared_ptr<graphics::Material> _material;
    std::shared_ptr<Primitive> _primitive;

  private:
    std::shared_ptr<CoreState> _coreState;
  };

}
