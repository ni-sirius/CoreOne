#include <stdafx.h>
#include <graphics/Material.h>
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
    _shader->SetMat4fv(modelMat, "ModelMat");
    _shader->SetMat4fv(viewMat, "ViewMat");
    _shader->SetMat4fv(projMat, "ProjectionMat");
    _shader->SetVec3f(cameraPos, "CameraPos");

    _shader->SetVec3f(_ambient, "material.ambient");
    _shader->Set1f(static_cast<float>(_shiness), "material.shiness");

    _shader->Set1i(_lightSensitive, "material.lightSensitive");
    _shader->Set1i(useColorFill, "material.useColors");
    _shader->Set1i(_diffuseTexUnit, "material.diffuseTex");
    _shader->Set1i(_specularTexUnit, "material.specularTex");

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
