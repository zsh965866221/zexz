#include "middle/camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace zexz {
namespace middle {

Camera::Camera(
    glm::vec3 position, 
    glm::vec3 up,
    glm::vec3 right,
    glm::vec3 front,
    float fovy,
    float aspect,
    float near,
    float far):
      position(position),
      up(up),
      right(right),
      front(front),
      fovy(fovy),
      aspect(aspect),
      near(near),
      far(far) {
  view = glm::lookAt(position, position + front, up);
  projection = glm::perspective(fovy, aspect, near, far);
}

Camera::Camera():
    Camera(
      glm::vec3(0.0f, 0.0f, -2.0f),
      glm::vec3(0.0f, 1.0f, 0.0f),
      glm::vec3(1.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 1.0f),
      45.0f,
      1.0f,
      0.1f,
      100.0f) {}

Camera::~Camera() {}

void Camera::lookat(const glm::vec3& position, const glm::vec3& target) {
  this->position = position;
  lookat(target);
}

void Camera::lookat(const glm::vec3& target) {
  front = glm::normalize(target - position);
  update();
}

void Camera::update() {
  onUpdate();

  view = glm::lookAt(position, position + front, up);
  projection = glm::perspective(fovy, aspect, near, far);

}

void Camera::onUpdate() {

}

} // namespace middle
} // namespace zexz