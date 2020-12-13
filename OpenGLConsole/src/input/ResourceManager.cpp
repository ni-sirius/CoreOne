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

std::shared_ptr<Shader> coreone::ResourceManager::GetShaderProgram(std::string tag)
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

