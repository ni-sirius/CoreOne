#include "stdafx.h"
#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 cameraUp):
  _viewMat(1.f),
  _position(position),
  _front(direction),
  _speed(3.f),
  _sensitivity(5.f),
  _up(cameraUp),
  _right(glm::cross(direction, cameraUp)),//cross upXdirection
  _pitch(0.f),
  _yaw(-90.f),
  _roll(0.f)
{
  
}

Camera::~Camera()
{
}

const glm::mat4 Camera::GetViewMatrix()
{
  updateCameraVectors();

  _viewMat = glm::lookAt(_position, _position + _front, _up);

  return _viewMat;
}

const glm::vec3 Camera::GetCameraPosition()
{
  return _position;
}

void Camera::UpdateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
{
  UpdateMouseInput(dt, offsetX, offsetY);
  UpdateKeyboardInput(dt, direction);
}

void Camera::UpdateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
{
  _pitch += static_cast<GLfloat>(offsetY * _sensitivity * dt);
  _yaw += static_cast<GLfloat>(offsetX * _sensitivity * dt);

  if (_pitch > 89.f)
    _pitch = 89.f;
  else if (_pitch < -89.f)
    _pitch = -89.f;

  if (_yaw > 360)
    _yaw = 0.f;
  if (_yaw < -360)
    _yaw = 0.f;
}

void Camera::UpdateKeyboardInput(const float& dt, const int direction)
{
  switch (direction)
  {
  case FORWARD:
    _position += _front * _speed * dt;
    break;
  case BACK:
    _position -= _front * _speed * dt;
    break;
  case LEFT:
    _position -= _right * _speed * dt;
    break;
  case RIGHT:
    _position += _right * _speed * dt;
    break;
  case UP:
    _position += _up * _speed * dt;
    break;
  case DOWN:
    _position -= _up * _speed * dt;
    break;

  default:
    break;
  }
}

void Camera::updateCameraVectors()
{
  _front.x = glm::cos(glm::radians(_pitch)) * glm::cos(glm::radians(_yaw));
  _front.y = glm::sin(glm::radians(_pitch));
  _front.z = glm::cos(glm::radians(_pitch)) * glm::sin(glm::radians(_yaw));

  _front = glm::normalize(_front);
  _right = glm::normalize(glm::cross(_front, _up));
}
