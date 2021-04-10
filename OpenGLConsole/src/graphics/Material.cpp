#include <stdafx.h>
#include <graphics/Material.h>
#include <graphics/Shader.h>
#include <lights/LightManager.h>

using namespace coreone::graphics;

Material::Material(glm::vec3 ambient,
  int shiness, bool lightSensitive)
  : _ambient(ambient)
  , _shiness(shiness)
  , _lightSensitive(lightSensitive)
{
}

void Material::SetMaterialState(glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projMat,
                                glm::vec3 cameraPos, std::shared_ptr<LightManager> lightMan) const
{
  if (_shader)
  {
    bool useColorFill = true;
    if (_diffuseTexture)
    {
      useColorFill = false;
      _diffuseTexture->Bind(_diffuseTexUnit);
    }

    if (_specularTexture)
    {
      _specularTexture->Bind(_specularTexUnit);
    }

    if (lightMan)
      lightMan->LoadLightsToShader(_shader);

    // TODO Rewrite uniform-set system
    _shader->SetUniform("ModelMat", modelMat);
    _shader->SetUniform("ViewMat", viewMat);
    _shader->SetUniform("ProjectionMat", projMat);
    _shader->SetUniform("CameraPos", cameraPos);

    _shader->SetUniform("material.ambient", _ambient);
    _shader->SetUniform("material.shiness", static_cast<float>(_shiness));

    _shader->SetUniform("material.lightSensitive", _lightSensitive);
    _shader->SetUniform("material.useColors", useColorFill);
    _shader->SetUniform("material.diffuseTex", _diffuseTexUnit);
    _shader->SetUniform("material.specularTex", _specularTexUnit);

    _binded = true;
    _shader->Use();
  }
}

void Material::UnsetMaterialState() const
{
  if (_binded && _shader)
  {
    if (_diffuseTexture)
    {
      _diffuseTexture->Unbind();
    }

    if (_specularTexture)
    {
      _specularTexture->Unbind();
    }

    _shader->Unuse();
    _binded = false;
  }
}
