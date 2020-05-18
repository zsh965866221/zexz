#pragma once

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
};

} // namespace middle
} // namespace zexz