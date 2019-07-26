#pragma once
#include <variant>
#include <unordered_map>


namespace light
{

  class LightContainer
  {
  public:
    using LightProp = std::variant<GLfloat, GLint, glm::fvec2, glm::fvec3, glm::fvec4, glm::mat3, glm::mat4>;

    LightContainer(std::string id);

    std::string GetId();

    int GetContainerNum();
    void SetContainerNum(int num);

    LightProp& GetPropByName(std::string name);
    void AddProperty(std::string id, LightProp prop);
    void RemoveProperty(std::string id);

    std::unordered_map<std::string, LightProp> Properties();

  protected:
    std::string _lightID;
    std::unordered_map<std::string, LightProp> _lightProperties;

    int _containerNumber;
  };

}
//class PointLight : public LightContainer
//{
//public:
//  PointLight() :
//    LightContainer("pointLight")
//  {
//    _lightProperties.insert({ "position", glm::vec3(-0.5f, -1.f, -0.5f) });
//
//    _lightProperties.insert({ "ambientColor", glm::vec3(-0.5f, -1.f, -0.5f) });
//    _lightProperties.insert({ "diffuseColor", glm::vec3(-0.5f, -1.f, -0.5f) });
//    _lightProperties.insert({ "specularColor", glm::vec3(-0.5f, -1.f, -0.5f) });
//
//    _lightProperties.insert({ "constant", 0.f });
//    _lightProperties.insert({ "linear", 0.f });
//    _lightProperties.insert({ "quadratic", 0.f });
//  }
//};

//class DirectionLight : public LightContainer
//{
//public:
//  DirectionLight() :
//    LightContainer("dirLight")
//  {
//    _lightProperties.insert({ "direction", glm::vec3(-0.5f, -1.f, -0.5f) });
//
//    _lightProperties.insert({ "ambientColor", glm::vec3(.1f, .1f, .1f) });
//    _lightProperties.insert({ "diffuseColor", glm::vec3(1.5f, 1.5f, 1.5f) });
//    _lightProperties.insert({ "specularColor", glm::vec3(1.f, 1.f, 1.f) });
//  }
//};
