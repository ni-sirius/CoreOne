#pragma once
class LightManager;

namespace coreone::graphics
{
  class Texture;
  class Shader;

  class Material
  {
  public:
    static std::shared_ptr<Material> CreateSimpleMaterial(
      std::shared_ptr<Shader> shader,
      std::shared_ptr<Texture> diffTex = nullptr,
      std::shared_ptr<Texture> specTex = nullptr,
      bool lightSensitive = true);

    Material(std::shared_ptr<Shader> shader, glm::vec3 ambient,
      int shiness, bool lightSensitive);
    ~Material() = default;

    void SetMaterialState(glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projMat, glm::vec3 cameraPos,
                          const LightManager& lightMan) const;
    void UnsetMaterialState() const;

    inline void SetDiffuseTexture(std::shared_ptr<Texture> diffTex, GLint unit) {
      _diffuseTexture = diffTex;
      _diffuseTexUnit = unit;
    };
    inline std::pair<std::shared_ptr<Texture>, GLint> GetDiffuseTexture() const {
      return std::make_pair(_diffuseTexture, _diffuseTexUnit);
    };
    inline void SetSpecularTexture(std::shared_ptr<Texture> specTex, GLint unit) {
      _specularTexture = specTex;
      _specularTexUnit = unit;
    };
    inline std::pair<std::shared_ptr<Texture>, GLint> GetSpecularTexture() const {
      return std::make_pair(_specularTexture, _specularTexUnit);
    };

    inline void SetLightSensitive(bool isSensitive) { _lightSensitive = isSensitive; }
    inline bool GetLightSensitive() { return _lightSensitive; };

    inline void SetShader(std::shared_ptr<Shader> shaderProgram) { _shader = shaderProgram; };
    inline std::shared_ptr<Shader> GetShader() const { return _shader; };

  private:
    std::shared_ptr<Shader> _shader = nullptr;

    glm::vec3 _ambient;
    int _shiness;

    std::shared_ptr<Texture> _diffuseTexture = nullptr;
    std::shared_ptr<Texture> _specularTexture = nullptr;

    GLint _diffuseTexUnit;
    GLint _specularTexUnit;

    bool _lightSensitive;

    mutable bool _binded = false;
  };

  using MaterialPtr = std::shared_ptr<Material>;
} //coreone::graphics
