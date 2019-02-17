#pragma once
#include <vec3.hpp>
#include <mat4x4.hpp>

class Camera
{
public:
  enum Direction {FORWARD, BACK, LEFT, RIGHT, UP, DOWN};

  Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 cameraUp);
  ~Camera();

  const glm::mat4 GetViewMatrix();
  const glm::vec3 GetCameraPosition();

  void UpdateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY);
  void UpdateMouseInput(const float& dt, const double& offsetX, const double& offsetY);
  void UpdateKeyboardInput(const float& dt, const int direction);

private:
  GLfloat _speed;
  GLfloat _sensitivity;

  glm::mat4 _viewMat;
  glm::vec3 _position;
  glm::vec3 _front;
  glm::vec3 _right;
  glm::vec3 _up;

  float _pitch;
  float _yaw;
  float _roll;

  void updateCameraVectors();
};

