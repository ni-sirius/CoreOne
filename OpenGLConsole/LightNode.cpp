#include "stdafx.h"
#include "LightNode.h"


LightNode::LightNode(glm::vec3 position, glm::vec3 color):
  _position(position),
  _color(color),
  _light(std::make_shared<PointLight>())
{
  updateModelMatrix();
}

void LightNode::Update(glm::mat4 modelMatrix /*= glm::mat4(1.f)*/)
{
  updateModelMatrix();

  _light->SetPosition(glm::vec3(modelMatrix * _modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f)));
  _light->SetColor(_color);

  CoreNode::Update(modelMatrix);
}

void LightNode::Render(Shader* shader, ShaderPass passType)
{
  if (passType == ShaderPass::LIGHT_PASS)
  {
    auto transfomedLightPos = glm::vec3(_modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f));

    shader->SetVec3f(transfomedLightPos, "light0.lightPos");
    shader->SetVec3f(_color, "light0.lightColor");
  }

  CoreNode::Render(shader, passType);
}

void LightNode::updateModelMatrix()
{
  _modelMatrix = glm::mat4(1.f);
  _modelMatrix = glm::translate(_modelMatrix, _position);
}
