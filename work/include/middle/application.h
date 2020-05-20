#pragma once

#include <functional>
#include <memory>
#include <string>

#include "glad/glad.h"
#include "gflags/gflags.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glog/logging.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "middle/shader.h"
#include "middle/texture.h"
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
    const int version_minor = 3,
    const glm::vec4& background_color = DEFAULT_BACKGROUND_COLOR);
  Application();
  virtual ~Application();

public:
  virtual bool init();
  virtual bool run();
  
public:
  virtual bool onGUI();
  virtual bool onInit();
  virtual bool onDraw();
  virtual bool onDestory();

public:
  // some call back from glfw
  virtual void framebuffer_size_callback(int width, int height);
  virtual void mouse_callback(float xpos, float ypos);
  virtual void scroll_callback(float xoffset, float yoffset);
  virtual void key_callback(int key, int scancode, int action, int mods);
  virtual void mouse_button_callback(int button, int action, int mods);

public:
  void register_onInit(std::function<bool(void)> func);
  void register_onGUI(std::function<bool(void)> func);
  void register_onDraw(std::function<bool(void)> func);
  void register_onDestory(std::function<bool(void)> func);
  void register_framebuffer_size_callback(std::function<void(int width, int height)> func);
  void register_mouse_callback(std::function<void(float xpos, float ypos)> func);
  void register_scroll_callback(std::function<void(int key, int scancode, int action, int mods)> func);
  void register_key_callback(std::function<void(int button, int action, int mods)> func);

protected:
  bool initGLFW();
  bool initIMGUI();
  bool cleanIMGUI();

  void error_callback(int error, const char* description);

protected:
  std::string window_name;
  int window_width;
  int window_height;
  int version_major;
  int version_minor;
  glm::vec4 background_color;
  std::string resource_dir;

  // single window
  GLFWwindow* window;

  float time;
  int frame;

public:
  std::function<bool(void)> func_onInit; 
  std::function<bool(void)> func_onGUI; 
  std::function<bool(void)> func_onDraw; 
  std::function<bool(void)> func_onDestory; 

  std::function<void(int width, int height)> func_framebuffer_size_callback;
  std::function<void(float xpos, float ypos)> func_mouse_callback;
  std::function<void(float xoffset, float yoffset)> func_scroll_callback;
  std::function<void(int key, int scancode, int action, int mods)> func_key_callback;
  std::function<void(int button, int action, int mods)> func_mouse_button_callback;
};

} // namespace middle
} // namespace zexz