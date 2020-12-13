#include <stdafx.h>
#include <graphics/Material.h>
#include <graphics/Shader.h>
#include <lights/LightManager.h>

using namespace coreone::graphics;

std::shared_ptr<Material> Material::CreateSimpleMaterial(
  std::shared_ptr<Shader> shader,
  std::shared_ptr<Texture> diffTex /*= nullptr*/,
  std::shared_ptr<Texture> specTex /*= nullptr*/,
  bool lightSensitive /*= true*/)
{
  auto material = std::make_shared<Material>(shader, glm::vec3(1.0f), 32, lightSensitive);
  material->SetDiffuseTexture(diffTex, 0);
  material->SetSpecularTexture(specTex, 1);

  return material;
}

Material::Material(std::shared_ptr<Shader> shader, glm::vec3 ambient,
  int shiness, bool lightSensitive)
  : _shader(shader)
  , _ambient(ambient)
  , _shiness(shiness)
  , _lightSensitive(lightSensitive)
{
}

void Material::SetMaterialState(glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projMat,
                                glm::vec3 cameraPos, const LightManager& lightMan) const
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

    lightMan.LoadLightsToShader(_shader);

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
