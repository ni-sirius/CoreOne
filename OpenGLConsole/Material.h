#pragma once
class Shader;

class Material
{
public:
  Material(glm::vec3 ambient,
           glm::vec3 diffuse,
           glm::vec3 specular,
           GLint diffudeTex,
           GLint specularTex);
  ~Material();

  void SendToShader(Shader& program);

private:
  glm::vec3 _ambient;
  glm::vec3 _diffuse;
  glm::vec3 _specular;
  GLint _diffudeTex;
  GLint _specularTex;
};
