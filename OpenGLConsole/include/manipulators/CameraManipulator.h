#pragma once
#include <memory>
#include <GL/GL.h>

namespace core
{
  class CameraNode;

  class CameraManipulator
  {
  public:
    CameraManipulator() = default;
    CameraManipulator(GLfloat speed, GLfloat sensitivity);

    enum Direction { FORWARD, BACK, LEFT, RIGHT, UP, DOWN };

    bool SetActiveCamera(std::shared_ptr<CameraNode> camera);

    void UpdateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY);
    void UpdateMouseInput(const float& dt, const double& offsetX, const double& offsetY);
    void UpdateKeyboardInput(const float& dt, const int direction);

    inline void SetFixed(bool fix) { _isFixed = fix; };

  private:
    std::shared_ptr<CameraNode> _camera;

    GLfloat _speed = 3.f;
    GLfloat _sensitivity = 5.f;
    bool _isFixed;
  };

} //namespace core