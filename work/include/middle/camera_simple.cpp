#include "middle/camera_simple.h"

#include "glm/gtc/matrix_transform.hpp"

namespace zexz {
namespace middle {

SimpleCamera::SimpleCamera(
  glm::vec3 position, 
  glm::vec3 lookat, 
  int window_width, 
  int window_height):
    Camera(
      position,
      glm::vec3(0.0f, 1.0f, 0.0f),
      glm::vec3(1.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 1.0f),
      45.0f,
      (float)window_width / (float)window_height,
      0.1f,
      100.0f
    ),
    basePosition(position),
    mousePosition(glm::vec2(0.0, 0.0)),
    mouseMidStart(glm::vec2(0.0, 0.0)),
    mouseMidOffset(glm::vec2(0.0, 0.0)),
    mouseMidOffsetTmp(glm::vec2(0.0, 0.0)),
    midPressed(false),
    distance(0.0f),
    window_width(window_width),
    window_height(window_height) {
  
}

SimpleCamera::~SimpleCamera() {}

void SimpleCamera::onUpdate() {
  position = basePosition + (front * distance);
  float x = mouseMidOffset.x / (float)window_width;
  float y = mouseMidOffset.y / (float)window_height;
  position += (right * x);
  position += (up * y);
}


void SimpleCamera::onMouseMotion(const SDL_Event* event) {
  int x = event->motion.x;
  int y = event->motion.y;
  mousePosition = glm::vec2(x, y);
  if (midPressed == true) {
    mouseMidOffset = mouseMidOffsetTmp + (mousePosition - mouseMidStart);
  }
}
void SimpleCamera::onMouseButton(const SDL_Event* event) {
  if (event->button.button == SDL_BUTTON_MIDDLE) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
      midPressed = true;
      mouseMidStart = mousePosition;
    } else if (event->type == SDL_MOUSEBUTTONUP) {
      midPressed = false;
      mouseMidOffsetTmp = mouseMidOffset;
      mouseMidStart = glm::vec2(0.0, 0.0);
    }
  }
}
void SimpleCamera::onKeyButton(const SDL_Event* event) {
  
}
void SimpleCamera::onMouseWheel(const SDL_Event* event) {
  distance += ((float)event->wheel.y * 0.1f);
}

void SimpleCamera::onSizeChanged(const SDL_Event* event) {
  int width = event->window.data1;
  int height = event->window.data2;
  aspect = float(width) / float(height);
}

void SimpleCamera::onEvent(const SDL_Event* event) {
  switch (event->type) {
  case SDL_MOUSEMOTION:
    onMouseMotion(event);
    break;
  case SDL_MOUSEBUTTONDOWN:
    onMouseButton(event);
    break;
  case SDL_MOUSEBUTTONUP:
    onMouseButton(event);
    break;
  case SDL_KEYDOWN:
    onKeyButton(event);
    break;
  case SDL_KEYUP:
    onKeyButton(event);
    break;
  case SDL_MOUSEWHEEL:
    onMouseWheel(event);
    break;
  case SDL_WINDOWEVENT:
    if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
      onSizeChanged(event);
    }
  default:
    break;
  }

  update();
}

} // namespace middle
} // namespace zexz