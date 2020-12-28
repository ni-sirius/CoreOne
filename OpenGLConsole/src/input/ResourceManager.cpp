#include <stdafx.h>
#include <input/ResourceManager.h>
#include <input/ResourceUploaders.h>
#include <graphics/Shader.h>
#include <graphics/Material.h>
#include <graphics/Texture.h>
#include <lights/LightNode.h>
#include <MeshNode.h>
#include <CoreNode.h>

using namespace coreone;
using namespace coreone::graphics;

std::shared_ptr<Shader> ResourceManager::LoadShaderProgram(std::string tag, const int versionMaj /*= 0*/, const int versionMin /*= 0*/,
  std::string vertexFile /*= ""*/, std::string fragmentFile /*= ""*/, std::string geometryFile /*= ""*/)
{
  auto it = _shaders.find(tag);
  if (it != _shaders.end())
  {
    return it->second;
  }

  const auto& vertexSource = vertexFile.empty() ? "" : loadShaderSource(vertexFile, versionMaj, versionMin);
  const auto& fragmentSource = fragmentFile.empty() ? "" : loadShaderSource(fragmentFile, versionMaj, versionMin);
  const auto& geomSource = geometryFile.empty() ? "" : loadShaderSource(geometryFile, versionMaj, versionMin);

  if (!vertexSource.empty() || !fragmentSource.empty() || !geomSource.empty())
  {
    auto shaderProgram = std::make_shared<Shader>(vertexSource, fragmentSource, geomSource);
    if (shaderProgram->MakeProgram())
    {
      _shaders.insert({ tag, shaderProgram });
      return shaderProgram;
    }
  }

  return nullptr;
}

std::shared_ptr<Shader> ResourceManager::GetShaderProgram(std::string tag)
{
  auto it = _shaders.find(tag);
  if (it != _shaders.end())
  {
    return it->second;
  }
  else
  {
    return nullptr;
  }
}

std::shared_ptr<Texture> ResourceManager::LoadTexture2D(std::string tag, std::string path)
{
  auto it = _textures.find(tag);
  if (it != _textures.end())
  {
    return it->second;
  }

  auto texture = std::make_shared<Texture>(path, GL_TEXTURE_2D);
  if (texture->Valid())
  {
    _textures.insert({ tag, texture });
    return texture;
  }

  return nullptr;
}

std::shared_ptr<Texture> ResourceManager::GetTexture(std::string tag)
{
  auto it = _textures.find(tag);
  if (it != _textures.end())
  {
    return it->second;
  }
  else
  {
    return nullptr;
  }
}

std::shared_ptr<graphics::Material> ResourceManager::CreateSimpleMaterial(std::string tag,
  std::string shaderTag, std::string diffuseTexTag, std::string specularTexTag, bool lightSensitive)
{
  auto it = _materials.find(tag);
  if (it != _materials.end())
  {
    return it->second;
  }

  auto shader = GetShaderProgram(shaderTag);
  if (shader)
  {
    auto diffTex = GetTexture(diffuseTexTag);
    auto specTex = GetTexture(specularTexTag);

    auto material = std::make_shared<Material>(glm::vec3(1.0f), 32, lightSensitive);
    material->SetShader(shader);
    material->SetDiffuseTexture(diffTex, 0);
    material->SetSpecularTexture(specTex, 1);

    _materials.insert({ tag, material });
    return material;
  }

  return nullptr;
}

std::shared_ptr<graphics::Material> ResourceManager::GetMaterial(std::string tag)
{
  auto it = _materials.find(tag);
  if (it != _materials.end())
  {
    return it->second;
  }
  else
  {
    return nullptr;
  }
}
