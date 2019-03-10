#pragma once
class Shader;

class Material
{
public:
  Material(glm::vec3 ambient,
           int shiness,
           GLint diffudeTex,
           GLint specularTex,
           std::shared_ptr<Shader> shader = nullptr);
  ~Material();

  void Use();
  inline void SetShader(std::shared_ptr<Shader> shaderProgram) { _shader = shaderProgram; };
  inline std::shared_ptr<Shader> GetShader() { return _shader; };

  inline void UseColors(bool use) { _useColors = use; }

  inline GLint GetDiffuseTexUnit() { return _diffuseTex; }
  inline GLint GetSpecularTexUnit() { return  _specularTex; }

private:
  glm::vec3 _ambient;
  int _shiness;

  bool _useColors;
  GLint _diffuseTex;
  GLint _specularTex;

  std::shared_ptr<Shader> _shader;
};
