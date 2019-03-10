#pragma once
#include "vec3.hpp"
#include "mat4x4.hpp"
#include "CoreNode.h"
#include "Lights.h"


class LightNode : public CoreNode
{
public:
  LightNode(glm::vec3 position, glm::vec3 color);
  virtual ~LightNode() = default;

  inline void SetPosition(const glm::vec3 position) { _position = position; };
  inline void SetColor(const glm::vec3 color) { _color = color; };

  //inline void SetLight(PointLight* light) { _light = std::shared_ptr<PointLight>(light); };
  inline std::shared_ptr<PointLight> GetLight() { return _light; };

  virtual void Update(glm::mat4 modelMatrix = glm::mat4(1.f)) override;
  virtual void Render(glm::mat4 viewMat, glm::mat4 projectionMat,
                      std::shared_ptr<Camera> camera,
                      std::vector<std::shared_ptr<PointLight>> pointLights) override;

private:
  void updateModelMatrix();

private:
  glm::vec3 _position;
  glm::vec3 _color;

  std::shared_ptr<PointLight> _light;
};

