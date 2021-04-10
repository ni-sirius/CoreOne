#pragma once
#include <unordered_map>
#include <nodes/Primitive.h>
#include <graphics/Texture.h>

class LightNodeBase;
class CoreNode;

namespace coreone
{
  class MeshNode;

  namespace graphics
  {
    class Shader;
    class Material;
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
    std::shared_ptr<graphics::Texture> CreateTexture(std::string tag, GLenum type, graphics::Texture::Format format, unsigned int width, unsigned int height);
    std::shared_ptr<graphics::Texture> GetTexture(std::string tag);

    std::shared_ptr<graphics::Material> CreateSimpleMaterial(std::string tag,
      std::string shaderTag, std::string diffuseTexTag = "", std::string specularTexTag = "", bool lightSensitive = true);
    std::shared_ptr<graphics::Material> GetMaterial(std::string tag);

    template <typename T = Quad>
    std::shared_ptr<MeshNode> CreateMeshWithPrimitive(std::string mesh_tag, std::string primitive_tag) {
      auto it = _meshNodes.find(mesh_tag);
      if (it != _meshNodes.end())
      {
        return it->second;
      }

      std::shared_ptr<Primitive> item;
      auto primitiveIt = _primitives.find(primitive_tag);
      if (primitiveIt == _primitives.end())
      {
        item = std::make_shared<T>();
        _primitives.insert({ primitive_tag, item });
      }
      else
      {
        item = primitiveIt->second;
      }

      auto meshNode = std::make_shared<MeshNode>(item);
      _meshNodes.insert({ mesh_tag, meshNode });

      return meshNode;
    };
    std::shared_ptr<Primitive> GetPrimitive(std::string tag);
    std::shared_ptr<MeshNode> GetMeshNode(std::string tag);

  private:
    std::unordered_map< std::string, std::shared_ptr<graphics::Shader> > _shaders;
    std::unordered_map< std::string, std::shared_ptr<graphics::Material> > _materials;
    std::unordered_map< std::string, std::shared_ptr<graphics::Texture> > _textures;

    std::unordered_map< std::string, std::shared_ptr<Primitive> > _primitives;
    std::unordered_map< std::string, std::shared_ptr<MeshNode> > _meshNodes;
  };
}
