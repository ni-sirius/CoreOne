#pragma once
#include "LightNode.h"

class SpotLightNode : public LightNodeBase
{
public:
  enum Attenuation {CONSTANT=0, LINEAR, QUADRATIC};

  SpotLightNode();
  SpotLightNode(glm::vec3 position,
                glm::vec3 direction,
                glm::vec3 ambientColor,
                glm::vec3 diffuseColor,
                glm::vec3 specularColor,
                float constantAtt,
                float linearAtt,
                float qudraticAtt,
                float cutoffAngle,
                float cutoffOuterAngle
  );
  virtual ~SpotLightNode() = default;

  inline void SetPosition(glm::vec3 position) { _position = position; };
  inline void SetDirection(glm::vec3 direction) { _direction = direction; };
  inline void SetAmbientColor(glm::vec3 color) { _ambientColor = color; };
  inline void SetDiffuseColor(glm::vec3 color) { _diffuseColor = color; };
  inline void SetSpecularColor(glm::vec3 color) { _specularColor = color; };
  inline void SetAttenuation(float attenuation, Attenuation type);
  inline void SetInnerConeAngle(float angle) { _cutoffAngle = angle; };
  inline void SetOuterConeAngle(float angle) { _cutoffOuterAngle = angle; };


  virtual void Update(const float& deltaTime, glm::mat4 modelMatrix = glm::mat4(1.f));

private:
  void updateModelMatrix();

private:
  glm::vec3 _position;
  glm::vec3 _direction;
  glm::vec3 _ambientColor;
  glm::vec3 _diffuseColor;
  glm::vec3 _specularColor;
  float _constantAtt;
  float _linearAtt;
  float _qudraticAtt;
  float _cutoffAngle;
  float _cutoffOuterAngle;
};
