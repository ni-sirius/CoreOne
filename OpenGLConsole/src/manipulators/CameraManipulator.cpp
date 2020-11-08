#include <stdafx.h>
#include <manipulators/CameraManipulator.h>
#include <nodes/CameraNode.h>

using namespace core;

core::CameraManipulator::CameraManipulator(GLfloat speed, GLfloat sensitivity)
  :_speed(speed)
  , _sensitivity(sensitivity)
{
}

bool CameraManipulator::SetActiveCamera(std::shared_ptr<CameraNode> camera)
{
  if (_camera != nullptr)
  {
    _camera = camera;
    return true;
  }

  return false;
}

void CameraManipulator::UpdateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
{
  UpdateMouseInput(dt, offsetX, offsetY);
  UpdateKeyboardInput(dt, direction);
}

void CameraManipulator::UpdateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
{
  if (_camera != nullptr && !_isFixed)
  {
    auto[pitch, yaw, roll] = _camera->GetCameraPose();
    pitch += static_cast<GLfloat>(offsetY * _sensitivity * dt);
    yaw += static_cast<GLfloat>(offsetX * _sensitivity * dt);

    if (pitch > 89.f)
      pitch = 89.f;
    else if (pitch < -89.f)
      pitch = -89.f;

    if (yaw > 360)
      yaw = 0.f;
    if (yaw < -360)
      yaw = 0.f;
  }
}

void CameraManipulator::UpdateKeyboardInput(const float& dt, const int direction)
{
  if (_camera != nullptr && !_isFixed)
  {
    auto position = _camera->GetCameraPosition();
    auto[right, up, front] = _camera->GetCameraOrientation();

    switch (direction)
    {
    case FORWARD:
      position += front * _speed * dt;
      break;
    case BACK:
      position -= front * _speed * dt;
      break;
    case LEFT:
      position -= right * _speed * dt;
      break;
    case RIGHT:
      position += right * _speed * dt;
      break;
    case UP:
      position += up * _speed * dt;
      break;
    case DOWN:
      position -= up * _speed * dt;
      break;

    default:
      break;
    }

    _camera->SetCameraPosition(position);
  }
}


