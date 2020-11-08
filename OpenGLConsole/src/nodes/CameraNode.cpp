#include <stdafx.h>
#include <nodes/CameraNode.h>

using namespace core;

core::CameraNode::CameraNode(glm::vec3 position, glm::vec3 direction, glm::vec3 cameraUp,
                              float fov, float nearPlane, float farPlane)
  :_viewMat(1.f),
  _projectionMat(1.f),
  _position(position),
  _front(direction),
  _up(cameraUp),
  _right(glm::cross(direction, cameraUp)),//cross upXdirection
  _pitch(0.f),
  _yaw(-90.f),
  _roll(0.f),
  _fov(0.f),
  _nearPlane(0.f),
  _farPlane(0.f)
{
}

const glm::mat4 CameraNode::GetViewMatrix()
{
  updateCameraVectors();

  _viewMat = glm::lookAt(_position, _position + _front, _up);

  return _viewMat;
}

const glm::mat4 core::CameraNode::GetProjectionMatrix()
{
  return {};
}


void CameraNode::updateCameraVectors()
{
  _front.x = glm::cos(glm::radians(_pitch)) * glm::cos(glm::radians(_yaw));
  _front.y = glm::sin(glm::radians(_pitch));
  _front.z = glm::cos(glm::radians(_pitch)) * glm::sin(glm::radians(_yaw));

  _front = glm::normalize(_front);
  _right = glm::normalize(glm::cross(_front, _up));
}

void core::CameraNode::Update(const float& deltaTime,
                              glm::mat4 modelMatrix /*= glm::mat4(1.f)*/)
{

}

void core::CameraNode::Render(glm::mat4 viewMat,
  glm::mat4 projectionMat,
  std::shared_ptr<Camera> camera,
  std::shared_ptr<LightManager> lightManager)
{

}
