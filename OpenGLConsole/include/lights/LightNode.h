#pragma once
#include "vec3.hpp"
#include "mat4x4.hpp"
#include "../CoreNode.h"
#include "Lights.h"


class LightNodeBase : public CoreNode
{
public:
  LightNodeBase(std::string name):
    _lightContainer(std::make_shared<light::LightContainer>(name)) {}
  virtual ~LightNodeBase() = default;

  inline std::shared_ptr<light::LightContainer> GetLightData() { return _lightContainer; }

  virtual void Update(const float& deltaTime, glm::mat4 modelMatrix = glm::mat4(1.f)) = 0;
  virtual void Render(glm::mat4 viewMat,
                      glm::mat4 projectionMat,
                      std::shared_ptr<Camera> camera,
                      std::shared_ptr<LightManager> lightManager) override {
    for (const auto& child : _childs)
    {
      child->Render(viewMat, projectionMat,
        camera, lightManager);
    }//Check movin to abstract class
  };

protected:
  std::shared_ptr<light::LightContainer> _lightContainer;
};
