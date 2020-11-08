#pragma once
#include <vec3.hpp>
#include <mat4x4.hpp>
#include <tuple>
#include <../CoreNode.h>

namespace core
{
  class CameraNode : public CoreNode
  {
  public:
    CameraNode(glm::vec3 position, glm::vec3 direction, glm::vec3 cameraUp, float fov, float nearPlane, float farPlane);
    ~CameraNode() = default;

    inline void SetCameraPosition(glm::vec3 position) { _position = position; };
    inline const glm::vec3 GetCameraPosition() { return _position; };
    inline void SetCameraPose(float pitch, float yaw, float roll) { _pitch = pitch; _yaw = yaw; _roll = roll; };
    inline std::tuple<float&, float&, float&> GetCameraPose() { return { _pitch, _yaw, _roll }; };
    inline std::tuple<glm::vec3&, glm::vec3&, glm::vec3&> GetCameraOrientation() { return { _right, _up, _front }; }

    const glm::mat4 GetViewMatrix();
    const glm::mat4 GetProjectionMatrix();

    void Update(const float& deltaTime, glm::mat4 modelMatrix = glm::mat4(1.f)) override;
    void Render(glm::mat4 viewMat, glm::mat4 projectionMat,
      std::shared_ptr<Camera> camera,
      std::shared_ptr<LightManager> lightManager) override;

  private:
    void updateCameraVectors();

    glm::mat4 _viewMat;
    glm::mat4 _projectionMat;

    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _right;
    glm::vec3 _up;

    float _pitch;
    float _yaw;
    float _roll;

    float _fov;
    float _nearPlane;
    float _farPlane;
  };

} //namespace core
