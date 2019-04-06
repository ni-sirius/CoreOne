#pragma once

class PointLight
{
public:
  PointLight(glm::vec3 position = glm::vec3(0.f), glm::vec3 color = glm::vec3(1.f));
  ~PointLight() = default;

  inline void SetPosition(glm::vec3 position) { _position = position; };
  inline glm::vec3 GetPosition() { return _position; };

  void SetColor(glm::vec3 color);
  glm::vec3 GetColor();

  inline void SetAmbientColor(glm::vec3 color) { _ambientColor = color; };
  inline void SetDiffuseColor(glm::vec3 color) { _diffuseColor = color; };
  inline void SetSpecularColor(glm::vec3 color) { _specularColor = color; };
  inline glm::vec3 GetAmbientColor() { return _ambientColor; };
  inline glm::vec3 GetDiffuseColor() { return _diffuseColor; };
  inline glm::vec3 GetSpecularColor() { return _specularColor; };

private:
  glm::vec3 _position;

  glm::vec3 _ambientColor;
  glm::vec3 _diffuseColor; //Main color
  glm::vec3 _specularColor;
};

