#pragma once
#include <list>
#include "mat4x4.hpp"

class Shader;
enum class ShaderPass;
class PointLight;

class CoreNode
{
public:
  CoreNode();
  virtual ~CoreNode();

  inline std::string GetUid() { return _uid; };
  std::shared_ptr<CoreNode> GetChildByUid(std::string uid);

  virtual std::string AddChild(std::shared_ptr<CoreNode> child);
  virtual void RemoveChild(std::string uid);

  virtual void Update(const float& deltaTime, glm::mat4 modelMatrix = glm::mat4(1.f));
  virtual void Render(glm::mat4 viewMat, glm::mat4 projectionMat,
                      std::shared_ptr<Camera> camera,
                      std::vector<std::shared_ptr<PointLight>> pointLights) = 0;

protected:
  inline void setUid(std::string uid) { _uid = uid; }
  inline void setLevel(int level) { _level = level; }

protected:
  std::list<std::shared_ptr<CoreNode> > _childs;
  glm::mat4 _modelMatrix;

private:
  int _level;
  std::string _uid;
};

