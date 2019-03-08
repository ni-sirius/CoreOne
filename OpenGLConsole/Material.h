#pragma once
class Shader;

class Material
{
public:
  Material(glm::vec3 ambient,
           int shiness,
           GLint diffudeTex,
           GLint specularTex);
  ~Material();

  void SendToShader(Shader& program);

  inline void UseColors(bool use) { _useColors = use; }

  inline GLint GetDiffuseTexUnit() { return _diffuseTex; }
  inline GLint GetSpecularTexUnit() { return  _specularTex; }

private:
  glm::vec3 _ambient;
  int _shiness;

  bool _useColors;
  GLint _diffuseTex;
  GLint _specularTex;
};
