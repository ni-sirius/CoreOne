#include "stdafx.h"
#include "lights/SpotLightNode.h"


SpotLightNode::SpotLightNode():
  SpotLightNode(glm::vec3(0.f, 2.0f, 0.f),
                glm::vec3(1.f, -1.0f, 0.f),
                glm::vec3(.1f, .1f, .1f),
                glm::vec3(1.5f, 1.5f, 1.5f),
                glm::vec3(1.f, 1.f, 1.f),
                1.f, 0.09f, 0.032f,
                12.5f, 13.5f)
{
}

SpotLightNode::SpotLightNode(glm::vec3 position,
                             glm::vec3 direction,
                             glm::vec3 ambientColor,
                             glm::vec3 diffuseColor,
                             glm::vec3 specularColor,
                             float constantAtt,
                             float linearAtt,
                             float qudraticAtt,
                             float cutoffAngle,
                             float cutoffOuterAngle) :
  LightNodeBase("spotLight"),
  _position(position),
  _direction(direction),
  _ambientColor(ambientColor),
  _diffuseColor(diffuseColor),
  _specularColor(specularColor),
  _constantAtt(constantAtt),
  _linearAtt(linearAtt),
  _qudraticAtt(qudraticAtt),
  _cutoffAngle(cutoffAngle),
  _cutoffOuterAngle(cutoffOuterAngle)
{
  _lightContainer->AddProperty("position", _position);
  _lightContainer->AddProperty("direction", _direction);

  _lightContainer->AddProperty("ambientColor", _ambientColor);
  _lightContainer->AddProperty("diffuseColor", _diffuseColor);
  _lightContainer->AddProperty("specularColor", _specularColor);

  _lightContainer->AddProperty("constant", _constantAtt);
  _lightContainer->AddProperty("linear", _linearAtt);
  _lightContainer->AddProperty("quadratic", _qudraticAtt);

  _lightContainer->AddProperty("cutoffAngle", glm::cos(glm::radians(_cutoffAngle)));
  _lightContainer->AddProperty("cutoffOuterAngle", glm::cos(glm::radians(_cutoffOuterAngle)));

  updateModelMatrix();
}

void SpotLightNode::SetAttenuation(float attenuation, Attenuation type)
{
  switch (type)
  {
  case SpotLightNode::CONSTANT:
    _constantAtt = attenuation;
    break;
  case SpotLightNode::LINEAR:
    _linearAtt = attenuation;
    break;
  case SpotLightNode::QUADRATIC:
    _qudraticAtt = attenuation;
    break;
  default:
    break;
  }
}

void SpotLightNode::Update(const float& deltaTime, glm::mat4 modelMatrix /*= glm::mat4(1.f)*/)
{
    updateModelMatrix();

    _lightContainer->GetPropByName("position") = _position;

    _lightContainer->GetPropByName("ambientColor") = _ambientColor;
    _lightContainer->GetPropByName("diffuseColor") = _diffuseColor;
    _lightContainer->GetPropByName("specularColor") = _specularColor;

    _lightContainer->GetPropByName("constant") = _constantAtt;
    _lightContainer->GetPropByName("linear") = _linearAtt;
    _lightContainer->GetPropByName("quadratic") = _qudraticAtt;

    _lightContainer->GetPropByName("cutoffAngle") = glm::cos(glm::radians(_cutoffAngle));
    _lightContainer->GetPropByName("cutoffOuterAngle") = glm::cos(glm::radians(_cutoffOuterAngle));

    CoreNode::Update(deltaTime, modelMatrix);
}

void SpotLightNode::updateModelMatrix()
{
  _modelMatrix = glm::mat4(1.f);
  _modelMatrix = glm::translate(_modelMatrix, _position);
}
