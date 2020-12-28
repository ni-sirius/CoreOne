#pragma once
#include <unordered_map>

class LightNodeBase;
class MeshNode;
class CoreNode;

namespace coreone
{
  namespace graphics
  {
    class Shader;
    class Material;
    class Texture;
  }

  class ResourceManager final
  {
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = default;
    ResourceManager& operator=(ResourceManager&) = default;
    ResourceManager(ResourceManager&&) = default;
    ResourceManager& operator=(ResourceManager&&) = default;

  public:
    ~ResourceManager() = default;

    static ResourceManager& Instance() {
      static ResourceManager instance;
      return instance;
    };

    std::shared_ptr<graphics::Shader> LoadShaderProgram(std::string tag, const int versionMaj = 0, const int versionMin = 0,
      std::string vertexFile = "", std::string fragmentFile = "", std::string geometryFile = "");
    std::shared_ptr<graphics::Shader> GetShaderProgram(std::string tag);

    std::shared_ptr<graphics::Texture> LoadTexture2D(std::string tag, std::string path);
    std::shared_ptr<graphics::Texture> GetTexture(std::string tag);

    std::shared_ptr<graphics::Material> CreateSimpleMaterial(std::string tag,
      std::string shaderTag, std::string diffuseTexTag = "", std::string specularTexTag = "", bool lightSensitive = true);
    std::shared_ptr<graphics::Material> GetMaterial(std::string tag);


  private:
    std::unordered_map< std::string, std::shared_ptr<graphics::Shader> > _shaders;
    std::unordered_map< std::string, std::shared_ptr<graphics::Material> > _materials;
    std::unordered_map< std::string, std::shared_ptr<graphics::Texture> > _textures;

    //TODO
    //std::unordered_map<std::string, std::shared_ptr<LightNodeBase> > _lights;
    //std::unordered_map<std::string, std::shared_ptr<MeshNode> > _meshes;
    //std::unordered_map<std::string, std::shared_ptr<CoreNode> > _windshields;
  };
}
