#include "stdafx.h"
#include "Material.h"

Material::Material(glm::vec3 ambient, int shiness, GLint diffudeTex, GLint specularTex):
  _ambient(ambient),
  _shiness(shiness),
  _useColors(false),
  _diffuseTex(diffudeTex),
  _specularTex(specularTex)
{
}

Material::~Material()
{
}

void Material::SendToShader(Shader& program)
{
  program.SetVec3f(_ambient, "material.ambient");
  program.Set1f(static_cast<float>(_shiness), "material.shiness");

  program.Set1i(_useColors, "material.useColors");
  program.Set1i(_diffuseTex, "material.diffuseTex");
  program.Set1i(_specularTex, "material.specularTex");
}
