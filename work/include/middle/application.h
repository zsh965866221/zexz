#pragma once

#include <functional>
#include <memory>
#include <string>

#include "glad/glad.h"
#include "gflags/gflags.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glog/logging.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "middle/animation.h"
#include "middle/shader.h"
#include "middle/texture.h"
#include "middle/timer.h"
#include "zexz/utils/utils.h"

namespace zexz {
namespace middle {
static const glm::vec4 DEFAULT_BACKGROUND_COLOR = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);

class Application {
public: 
  Application(
    const std::string& window_name, 
    const int window_width,
    const int window_height,
    const int version_major = 3,
    const int version_minor = 0,
    const glm::vec4& background_color = DEFAULT_BACKGROUND_COLOR);
  Application();
  virtual ~Application();

public:
  virtual bool init();
  virtual bool run();
  
public:
  virtual bool onInit();
  virtual bool onEvent(const SDL_Event* event);
  virtual bool onGUI();
  virtual bool onDraw();
  virtual bool onDestroy();
  virtual bool onUpdate();

public:
  void onInit(std::function<bool(Application* self)> func);
  void onEvent(std::function<bool(Application* self, const SDL_Event* event)> func);
  void onGUI(std::function<bool(Application* self)> func);
  void onDraw(std::function<bool(Application* self)> func);
  void onDestroy(std::function<bool(Application* self)> func);
  void onUpdate(std::function<bool(Application* self)> func);

private:
  bool initSDL();
  bool initIMGUI();
  bool cleanIMGUI();

public:
  virtual void onGUIAnimation();

public:
  std::string window_name;
  int window_width;
  int window_height;
  int version_major;
  int version_minor;
  glm::vec4 background_color;
  std::string resource_dir;

  // single window
  SDL_Window* window;
  SDL_GLContext gl_context;

  float time;
  int frame;

public:
  utils::Timer timer;
  animation::Animation animation;

public:
  std::function<bool(Application* self)> func_onInit;
  std::function<bool(Application* self, const SDL_Event* event)> func_onEvent;
  std::function<bool(Application* self)> func_onGUI;
  std::function<bool(Application* self)> func_onDraw;
  std::function<bool(Application* self)> func_onDestroy;
  std::function<bool(Application* self)> func_onUpdate;
};

} // namespace middle
} // namespace zexz