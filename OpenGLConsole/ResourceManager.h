#pragma once


class ResourceManager final
{
  ResourceManager();
  ResourceManager(const ResourceManager&) = default;
  ResourceManager& operator=(ResourceManager&) = default;
  ResourceManager(ResourceManager&&) = default;
  ResourceManager& operator=(ResourceManager&&) = default;

public:
  ~ResourceManager();

  static ResourceManager& Instance() {
    static ResourceManager instance;
    return instance;
  };
};

