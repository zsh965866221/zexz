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

class Application {
public: 
  Application(
    const std::string& name, 
    const int window_width, 
    const int window_height,
    const int version_major = 3,
    const int version_minor = 3);
  Application();
  virtual ~Application();

public:
  virtual void run();
  
public:
  virtual void onGUI();
  virtual void onInit();
  virtual void onDraw();
  virtual void onDestory();

public:
  // some call back from glfw
  virtual void framebuffer_size_callback(int width, int height);
  virtual void mouse_callback(float xpos, float ypos);
  virtual void scroll_callback(float xoffset, float yoffset);
  virtual void key_callback(int key, int scancode, int action, int mods);
  virtual void mouse_button_callback(int button, int action, int mods);

private:
  void initGlfw();

private:
  std::string name;
  int window_width;
  int window_height;
  std::string resource_dir;

  // single window
  std::unique_ptr<GLFWwindow> window;

  float time;
  int frame;
};

} // namespace middle
} // namespace zexz