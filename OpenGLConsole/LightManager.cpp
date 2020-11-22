#include "stdafx.h"
#include "lights/LightManager.h"


using namespace light;

LightManager::LightManager()
{
}


LightManager::~LightManager()
{
}

void LightManager::RegisterLight(std::shared_ptr<LightContainer> light)
{
  const auto name = light->GetId();
  _lights[name].push_back(light);
}

void LightManager::LoadLightsToShader(std::shared_ptr<Shader> shader) const
{
  for (const auto& lightType : _lights)
  {
    const std::string lightName = lightType.first;

    size_t lightCount(0);
    for (const auto& light : lightType.second)
    {
      for (const auto& lightProp : light->Properties())
      {
        std::string lightPropertyName = lightProp.first;

        std::string lightUniformName = lightName + "[" + std::to_string(lightCount) + "]." + lightPropertyName;

        std::visit([lightUniformName, shader](auto& value) { shader->SetUniform(lightUniformName.c_str(), value); }, lightProp.second);
      }

      lightCount++;
    }
  }

}
