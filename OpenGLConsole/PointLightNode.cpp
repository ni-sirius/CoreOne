#include "stdafx.h"
#include "lights/PointLightNode.h"


PointLightNode::PointLightNode():
  PointLightNode(glm::vec3(-0.5f, -1.f, -0.5f),
                 glm::vec3(.1f, .1f, .1f),
                 glm::vec3(1.5f, 1.5f, 1.5f),
                 glm::vec3(1.f, 1.f, 1.f),
                 1.f, 0.09f, 0.032f)
{
}

PointLightNode::PointLightNode(glm::vec3 position,
                               glm::vec3 ambientColor,
                               glm::vec3 diffuseColor,
                               glm::vec3 specularColor,
                               float constantAtt,
                               float linearAtt,
                               float qudraticAtt):
  LightNodeBase("pointLight"),
  _position(position),
  _ambientColor(ambientColor),
  _diffuseColor(diffuseColor),
  _specularColor(specularColor),
  _constantAtt(constantAtt),
  _linearAtt(linearAtt),
  _qudraticAtt(qudraticAtt)
{
  _lightContainer->AddProperty("position", _position);

  _lightContainer->AddProperty("ambientColor", _ambientColor);
  _lightContainer->AddProperty("diffuseColor", _diffuseColor);
  _lightContainer->AddProperty("specularColor", _specularColor);

  _lightContainer->AddProperty("constant", _constantAtt);
  _lightContainer->AddProperty("linear", _linearAtt);
  _lightContainer->AddProperty("quadratic", _qudraticAtt);

  updateModelMatrix();
}

void PointLightNode::SetAttenuation(float attenuation, Attenuation type)
{
  switch (type)
  {
  case PointLightNode::CONSTANT:
    _constantAtt = attenuation;
    break;
  case PointLightNode::LINEAR:
    _linearAtt = attenuation;
    break;
  case PointLightNode::QUADRATIC:
    _qudraticAtt = attenuation;
    break;
  default:
    break;
  }
}

void PointLightNode::Update(const float& deltaTime, glm::mat4 modelMatrix /*= glm::mat4(1.f)*/)
{
    updateModelMatrix();

    _lightContainer->GetPropByName("position") = _position;

    _lightContainer->GetPropByName("ambientColor") = _ambientColor;
    _lightContainer->GetPropByName("diffuseColor") = _diffuseColor;
    _lightContainer->GetPropByName("specularColor") = _specularColor;

    _lightContainer->GetPropByName("constant") = _constantAtt;
    _lightContainer->GetPropByName("linear") = _linearAtt;
    _lightContainer->GetPropByName("quadratic") = _qudraticAtt;

    CoreNode::Update(deltaTime, modelMatrix);
}

void PointLightNode::updateModelMatrix()
{
  _modelMatrix = glm::mat4(1.f);
  _modelMatrix = glm::translate(_modelMatrix, _position);
}
