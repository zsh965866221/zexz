#pragma once

#include "glm/glm.hpp"

namespace zexz {
namespace middle {

class Camera {
public:
  Camera(
    glm::vec3 position = glm::vec3(0.0f, 0.0f, -2.0f), 
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f),
    float fovy = 45.0f,
    float aspect = 1.0f,
    float near = 0.1f,
    float far = 100.0f);
  Camera();
  virtual ~Camera();

public:
  virtual void update();
  virtual void lookat(const glm::vec3& target);
  virtual void lookat(const glm::vec3& poistion, const glm::vec3& target);

public:
  virtual void onUpdate();

public:
  glm::vec3 position;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 front;
  float fovy;
  float aspect;
  float near;
  float far;

  glm::mat4 view;
  glm::mat4 projection;

};


} // namespace middle
} // namespace zexz