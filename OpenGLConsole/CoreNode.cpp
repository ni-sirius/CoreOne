#include "stdafx.h"
#include "CoreNode.h"


CoreNode::CoreNode():
  _modelMatrix(1.f),
  _level(-1),
  _uid("")
{
}

CoreNode::~CoreNode()
{
}

std::shared_ptr<CoreNode> CoreNode::GetChildByUid(std::string uid)
{
  std::shared_ptr<CoreNode> result;

  auto it = std::find_if(_childs.begin(), _childs.end(),
              [&uid](auto& child) { return child->GetUid() == uid; });
  if (it != _childs.end())
  {
    result = *it;
  }

  return result;
}

std::string CoreNode::AddChild(std::shared_ptr<CoreNode> child)
{
  if (child)
  {
    std::string uid = std::to_string(_level) + ":" + std::to_string(_childs.size());

    child->setUid(uid);
    _childs.push_back(child);

    return uid;
  }

  return "";
}

void CoreNode::RemoveChild(std::string uid)
{
  auto it = std::find_if(_childs.begin(), _childs.end(),
              [&uid](auto& child) { return child->GetUid() == uid; });
  if (it != _childs.end())
  {
    _childs.erase(it);
  }
}

void CoreNode::Update(glm::mat4 modelMatrix /*= glm::mat4(1.f)*/)
{
  _modelMatrix = modelMatrix * _modelMatrix;

  for (const auto& child : _childs)
  {
    child->Update(_modelMatrix);
  }
}
