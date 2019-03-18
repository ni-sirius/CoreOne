#include "stdafx.h"
#include "CoreCommands.h"
#include "MeshNode.h"
#include "LightNode.h"

SetMeshNodePositionCommand::SetMeshNodePositionCommand(glm::vec3 position,
                                                       std::shared_ptr<MeshNode> mesh):
  _position(position),
  _mesh(mesh)
{
}

void SetMeshNodePositionCommand::Execute()
{
  _mesh->SetPosition(_position);
}

SetLightNodePositionCommand::SetLightNodePositionCommand(glm::vec3 position,
                                                         std::shared_ptr<LightNode> lightNode):
  _position(position),
  _lightNode(lightNode)
{
}

void SetLightNodePositionCommand::Execute()
{
  _lightNode->SetPosition(_position);
}

SetCameraPositionCommand::SetCameraPositionCommand(
  glm::vec3 position,
  std::shared_ptr<Camera> camera):
  _position(position),
  _camera(camera)
{
}

void SetCameraPositionCommand::Execute()
{
  _camera->SetCameraPosition(_position);
}
