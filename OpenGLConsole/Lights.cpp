#include "stdafx.h"
#include "Lights.h"

PointLight::PointLight(glm::vec3 position /*= glm::vec3(0.f)*/, glm::vec3 color /*= glm::vec3(1.f)*/) :
  _position(position),
  _color(color)
{
}
