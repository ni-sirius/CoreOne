#include "stdafx.h"
#include "MeshNode.h"
#include "lights/Lights.h"
#include "states/CoreState.h"
#include "graphics/Texture.h"
#include "nodes/Primitive.h"

using namespace coreone;

MeshNode::MeshNode(std::shared_ptr<Primitive> primitive,
                   std::shared_ptr<graphics::Material> material /*= nullptr*/):
  _material(material),
  _primitive(primitive)
{
  updateModelMatrix();
}

MeshNode::~MeshNode()
{
}

void MeshNode::Update(const float& deltaTime, glm::mat4 modelMatrix /*= glm::mat4(1.f)*/)
{
  updateModelMatrix();

  CoreNode::Update(deltaTime, modelMatrix);
}

void MeshNode::Render(glm::mat4 viewMat, glm::mat4 projectionMat,
                      std::shared_ptr<Camera> camera,
                      std::shared_ptr<LightManager> lightManager)
{
  if (!_material || !_primitive)
    return; // Change later, maybe use default shader/material

  if (_visible)
  {
    //Using of State Set
    if (_coreState)
      _coreState->setState();

    glm::vec3 cameraPos{ 0.f };
    if (camera)
      cameraPos = camera->GetCameraPosition();

    _material->SetMaterialState(_modelMatrix, viewMat, projectionMat, cameraPos, lightManager);

    //Main draw call
    _primitive->DrawPrimitive();

    _material->UnsetMaterialState();

    //Using of State Set
    if (_coreState)
      _coreState->restoreState();
  }

  //Render childs
  for (const auto& child : _childs)
  {
    child->Render(viewMat, projectionMat,
                  camera, lightManager);
  }
}

CoreState* const MeshNode::GetOrCreateCoreState()
{
  if (!_coreState)
    _coreState = std::make_shared<CoreState>();

  return _coreState.get();
}

void MeshNode::SetCoreState(std::shared_ptr<CoreState> state)
{
  _coreState = state;
}

void MeshNode::updateModelMatrix()
{
  _modelMatrix = glm::mat4(1.f);
  _modelMatrix = glm::translate(_modelMatrix, _position);
  _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.x), glm::vec3(1.f, 0.f, 0.f));
  _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.y), glm::vec3(0.f, 1.f, 0.f));
  _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.z), glm::vec3(0.f, 0.f, 1.f));
  _modelMatrix = glm::scale(_modelMatrix, _scale);
}
