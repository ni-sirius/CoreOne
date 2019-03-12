#pragma once
#include <vec3.hpp>


class MeshNode;
class LightNode;

//================================Base Commands========================================
class CoreBaseCommand
{
public:
  virtual void Execute() = 0;
};

class CoreSingleCommand : public CoreBaseCommand
{
public:
  virtual void Execute() override = 0;
};

class CoreMacroCommand final : public CoreBaseCommand
{
public:
  inline virtual void Execute() override {
    for (const auto& command : _commands)
      command->Execute();
  }

  inline void AddCommand(CoreBaseCommand* command){ _commands.emplace_back(command); }

private:
  std::vector<std::unique_ptr<CoreBaseCommand>> _commands;
};

//============================Custom Commands==========================================

class SetMeshNodePositionCommand : public CoreSingleCommand
{
public:
  SetMeshNodePositionCommand(glm::vec3 position, std::shared_ptr<MeshNode> mesh);

  virtual void Execute() override;

private:
  glm::vec3 _position;
  std::shared_ptr<MeshNode> _mesh;
};

class SetLightNodePositionCommand : public CoreSingleCommand
{
public:
  SetLightNodePositionCommand(glm::vec3 position, std::shared_ptr<LightNode> lightNode);

  virtual void Execute() override;

private:
  glm::vec3 _position;
  std::shared_ptr<LightNode> _lightNode;
};
