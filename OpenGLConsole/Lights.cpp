#include "stdafx.h"
#include "Lights.h"

PointLight::PointLight(glm::vec3 position /*= glm::vec3(0.f)*/, glm::vec3 color /*= glm::vec3(1.f)*/) :
  _position(position)
{
  SetColor(color);
}

void PointLight::SetColor(glm::vec3 color)
{
  _diffuseColor = color * _diffConst;
  _ambientColor = _diffuseColor * _ambientConst;
  _specularColor = glm::vec3(_specConst);
}

glm::vec3 PointLight::GetColor()
{
  return _diffuseColor * _diffConst * 4.f;
}
