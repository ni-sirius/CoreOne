#include "stdafx.h"
#include "Material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shiness, GLint diffudeTex, GLint specularTex):
  _ambient(ambient),
  _diffuse(diffuse),
  _specular(specular),
  _shiness(shiness),
  _diffudeTex(diffudeTex),
  _specularTex(specularTex)
{

}

Material::~Material()
{

}

void Material::SendToShader(Shader& program)
{
  program.SetVec3f(_ambient, "material.ambient");
  program.SetVec3f(_diffuse, "material.diffuse");
  program.SetVec3f(_specular, "material.specular");
  program.Set1f(_shiness, "material.shiness");
  program.Set1i(_diffudeTex, "material.diffuseTex");
  program.Set1i(_specularTex, "material.specularTex");
}
