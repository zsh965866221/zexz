#pragma once

#include "SDL.h"

#include "middle/camera.h"

namespace zexz {
namespace middle {

class SimpleCamera: public Camera {
public:
  SimpleCamera(
    glm::vec3 position, 
    glm::vec3 lookat, 
    int window_width, 
    int window_height);
  SimpleCamera();
  virtual ~SimpleCamera();

public:
  void onUpdate();

public:
  void onEvent(const SDL_Event* event);
  void onMouseMotion(const SDL_Event* event);
  void onMouseButton(const SDL_Event* event);
  void onKeyButton(const SDL_Event* event);
  void onMouseWheel(const SDL_Event* event);

public:
  int window_width;
  int window_height;
  glm::vec3 basePosition;

public:
  glm::vec2 mousePosition;
  glm::vec2 mouseMidStart;
  glm::vec2 mouseMidOffset;
  glm::vec2 mouseMidOffsetTmp;
  bool midPressed;

  float distance;

};





} // namespace middle
} // namespace zexz