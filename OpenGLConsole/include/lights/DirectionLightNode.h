#pragma once
#include "LightNode.h"


class DirectionLightNode : public LightNodeBase
{
public:
  DirectionLightNode();
  DirectionLightNode(glm::vec3 direction,
                     glm::vec3 ambientColor,
                     glm::vec3 diffuseColor,
                     glm::vec3 specularColor
                    );
  virtual ~DirectionLightNode() = default;

  inline void SetDirection(glm::vec3 direction) { _direction = direction; };
  inline void SetAmbientColor(glm::vec3 color) { _ambientColor = color; };
  inline void SetDiffuseColor(glm::vec3 color) { _diffuseColor = color; };
  inline void SetSpecularColor(glm::vec3 color) { _specularColor = color; };

  virtual void Update(const float& deltaTime, glm::mat4 modelMatrix = glm::mat4(1.f));

private:
  glm::vec3 _direction;
  glm::vec3 _ambientColor;
  glm::vec3 _diffuseColor;
  glm::vec3 _specularColor;
};
