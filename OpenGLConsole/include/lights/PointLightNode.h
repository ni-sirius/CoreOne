#pragma once
#include "LightNode.h"

class PointLightNode : public LightNodeBase
{
public:
  enum Attenuation {CONSTANT=0, LINEAR, QUADRATIC};

  PointLightNode();
  PointLightNode(glm::vec3 position,
                 glm::vec3 ambientColor,
                 glm::vec3 diffuseColor,
                 glm::vec3 specularColor,
                 float constantAtt,
                 float linearAtt,
                 float qudraticAtt
  );
  virtual ~PointLightNode() = default;

  inline void SetPosition(glm::vec3 position) { _position = position; };
  inline void SetAmbientColor(glm::vec3 color) { _ambientColor = color; };
  inline void SetDiffuseColor(glm::vec3 color) { _diffuseColor = color; };
  inline void SetSpecularColor(glm::vec3 color) { _specularColor = color; };
  inline void SetAttenuation(float attenuation, Attenuation type);

  virtual void Update(const float& deltaTime, glm::mat4 modelMatrix = glm::mat4(1.f));

private:
  void updateModelMatrix();

private:
  glm::vec3 _position;
  glm::vec3 _ambientColor;
  glm::vec3 _diffuseColor;
  glm::vec3 _specularColor;
  float _constantAtt;
  float _linearAtt;
  float _qudraticAtt;
};
