#include "stdafx.h"
#include "CoreCommands.h"
#include "MeshNode.h"
#include "lights/LightNode.h"
#include "lights/PointLightNode.h"
#include "TextNode.h"

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
                                                         std::shared_ptr<PointLightNode> lightNode):
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

SetTextScaleCommand::SetTextScaleCommand(float scale, std::shared_ptr<TextNode> text):
  _scale(scale),
  _text(text)
{
}

void SetTextScaleCommand::Execute()
{
  _text->SetScale(_scale);
}

SetMeshNodeVisibleCommand::SetMeshNodeVisibleCommand(bool visible, std::shared_ptr<MeshNode> mesh):
  _visible(visible),
  _mesh(mesh)
{
}

void SetMeshNodeVisibleCommand::Execute()
{
  _mesh->SetVisible(_visible);
}

SetTextNodeVisibleCommand::SetTextNodeVisibleCommand(bool visible, std::shared_ptr<TextNode> text):
  _visible(visible),
  _text(text)
{
}

void SetTextNodeVisibleCommand::Execute()
{
  _text->SetVisible(_visible);
}
