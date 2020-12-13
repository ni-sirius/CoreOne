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


  private:
    std::unordered_map< std::string, std::shared_ptr<graphics::Shader> > _shaders;
    std::unordered_map< std::string, std::shared_ptr<graphics::Material> > _materials;
    std::unordered_map< std::string, std::shared_ptr<graphics::Texture> > _textures;

    std::unordered_map<std::string, std::shared_ptr<LightNodeBase> > _lights;
    std::unordered_map<std::string, std::shared_ptr<MeshNode> > _meshes;
    std::unordered_map<std::string, std::shared_ptr<CoreNode> > _windshields;
  };
}
