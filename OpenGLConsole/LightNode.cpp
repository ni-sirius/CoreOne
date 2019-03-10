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

void LightNode::Render(glm::mat4 viewMat, glm::mat4 projectionMat,
                       std::shared_ptr<Camera> camera,
                       std::vector<std::shared_ptr<PointLight>> pointLights)
{
  for (const auto& child : _childs)
  {
    child->Render(viewMat, projectionMat,
                  camera, pointLights);
  }
}

void LightNode::updateModelMatrix()
{
  _modelMatrix = glm::mat4(1.f);
  _modelMatrix = glm::translate(_modelMatrix, _position);
}
