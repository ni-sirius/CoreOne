#include "stdafx.h"
#include "Material.h"

Material::Material(glm::vec3 ambient,
                   int shiness,
                   GLint diffudeTex,
                   GLint specularTex,
                   std::shared_ptr<Shader> shader /*= nullptr*/):
  _ambient(ambient),
  _shiness(shiness),
  _useColors(false),
  _diffuseTex(diffudeTex),
  _specularTex(specularTex),
  _shader(shader)
{
}

Material::~Material()
{
}

void Material::Use()
{
  if (_shader)
  {
    _shader->SetVec3f(_ambient, "material.ambient");
    _shader->Set1f(static_cast<float>(_shiness), "material.shiness");

    _shader->Set1i(_useColors, "material.useColors");
    _shader->Set1i(_diffuseTex, "material.diffuseTex");
    _shader->Set1i(_specularTex, "material.specularTex");
  }
}
