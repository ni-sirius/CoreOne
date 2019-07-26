#include "stdafx.h"
#include "lights/DirectionLightNode.h"


using namespace light;

DirectionLightNode::DirectionLightNode():
  DirectionLightNode(glm::vec3(-1.f, .0f, .0f),//TODO Remove Test Values
                     glm::vec3(.1f, .1f, .1f),
                     glm::vec3(1.5f, 1.5f, 1.5f),
                     glm::vec3(1.f, 1.f, 1.f))
{
}

DirectionLightNode::DirectionLightNode(glm::vec3 direction,
                                       glm::vec3 ambientColor,
                                       glm::vec3 diffuseColor,
                                       glm::vec3 specularColor):
  LightNodeBase("dirLight"),
  _direction(direction),
  _ambientColor(ambientColor),
  _diffuseColor(diffuseColor),
  _specularColor(specularColor)
{
  _lightContainer->AddProperty("direction", _direction);

  _lightContainer->AddProperty( "ambientColor", _ambientColor);
  _lightContainer->AddProperty( "diffuseColor", _diffuseColor);
  _lightContainer->AddProperty( "specularColor", _specularColor);
}

void DirectionLightNode::Update(const float& deltaTime, glm::mat4 modelMatrix /*= glm::mat4(1.f)*/)
{
  _lightContainer->GetPropByName("direction") = _direction;

  _lightContainer->GetPropByName("ambientColor") = _ambientColor;
  _lightContainer->GetPropByName("diffuseColor") = _diffuseColor;
  _lightContainer->GetPropByName("specularColor") = _specularColor;

  CoreNode::Update(deltaTime, modelMatrix);
}
