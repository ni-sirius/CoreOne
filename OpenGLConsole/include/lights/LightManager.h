#pragma once
#include "lights/Lights.h"

namespace coreone::graphics
{
  class Shader;
}

class LightManager
{
public:
  LightManager();
  ~LightManager();

  void RegisterLight(std::shared_ptr<light::LightContainer> light);

  void LoadLightsToShader(std::shared_ptr<coreone::graphics::Shader> shader) const;

private:
  std::unordered_map<std::string, std::list<std::shared_ptr<light::LightContainer>>> _lights;
};
