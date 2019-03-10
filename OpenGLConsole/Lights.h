#pragma once

class PointLight
{
public:
  PointLight(glm::vec3 position = glm::vec3(0.f), glm::vec3 color = glm::vec3(1.f));
  ~PointLight() = default;

  inline void SetPosition(glm::vec3 position) { _position = position; };
  inline glm::vec3 GetPosition() { return _position; };

  inline void SetColor(glm::vec3 color) { _color = color; };
  inline glm::vec3 GetColor() { return _color; };

private:
  glm::vec3 _position;
  glm::vec3 _color;
};

