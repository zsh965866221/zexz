#include "middle/application.h"

#include <fstream>
#include <sstream>
#include <string>

#include "glog/logging.h"

namespace zexz {
namespace middle {

static void glfw_error_callback(int error, const char* description);
static void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void glfw_mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

Application::Application(
    const std::string& window_name, 
    const int window_width, 
    const int window_height,
    const int version_major,
    const int version_minor,
    const glm::vec4& background_color): 
      window_name(window_name), 
      window_width(window_width),
      window_height(window_height),
      version_major(version_major),
      version_minor(version_minor),
      background_color(background_color),
      window(nullptr),
      func_onInit(nullptr),
      func_onGUI(nullptr),
      func_onDraw(nullptr),
      func_onDestory(nullptr),
      func_framebuffer_size_callback(nullptr),
      func_mouse_callback(nullptr),
      func_scroll_callback(nullptr),
      func_key_callback(nullptr),
      func_mouse_button_callback(nullptr) {
  resource_dir = zexz::utils::getResourcesDir();
}

Application::Application(): 
    Application(
      "zexz",
      1080,
      960
    ) {}

Application::~Application() {
  cleanIMGUI();
  onDestory();
  if (func_onDestory != nullptr) {
    func_onDestory();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
}
bool Application::initGLFW() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(
    window_width,
    window_height,
    window_name.c_str(),
    nullptr,
    nullptr
  );
  if (window == nullptr) {
    LOG(ERROR) << "[ERROR]: Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));

  glfwSetErrorCallback(glfw_error_callback);
  glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
  glfwSetCursorPosCallback(window, glfw_mouse_callback);
  glfwSetScrollCallback(window, glfw_scroll_callback);
  glfwSetKeyCallback(window, glfw_key_callback);
  glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);

  // load glad opengl interface
  if (!gladLoadGL()) {
    LOG(ERROR) << "[ERROR]: Failed to init glad opengl interface" << std::endl;
    glfwTerminate();
    return false;
  }
  return true;
}

bool Application::initIMGUI() {
  // init IMGUI
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  // style
  ImGui::StyleColorsDark();
  // render
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 300 es");
  return true;
}

bool Application::init() {
  bool ret;
  ret = initGLFW();
  CHECK_EQ(ret, true);
  ret = initIMGUI();
  CHECK_EQ(ret, true);

  // init
  onInit();
  if (func_onInit != nullptr) {
    func_onInit();
  }

  return ret;
}

bool Application::run() {
  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // timer and frame
    time = timer.delta();
    frame++;
    animation.update();
    
    //ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    onGUIAnimation();
    onGUI();
    if(func_onGUI != nullptr) {
      func_onGUI();
    }
    ImGui::Render();

    glClearColor(
      background_color.r,
      background_color.g,
      background_color.b, 
      background_color.a
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw
    onDraw();
    if (func_onDraw != nullptr) {
      func_onDraw();
    }
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // swap buffer
    glfwSwapBuffers(window);
  }
  return true;
}

bool Application::cleanIMGUI() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  return true;
}

bool Application::onGUI() {
  return true;
}

bool Application::onInit() {
  return true;
}

bool Application::onDraw() {
  return true;
}

bool Application::onDestory() {
  return true;
}

void Application::framebuffer_size_callback(int width, int height) {
  window_width = width;
  window_height = height;
  glViewport(0, 0, width, height);
}
void Application::mouse_callback(float xpos, float ypos) {

}
void Application::scroll_callback(float xoffset, float yoffset) {

}
void Application::key_callback(int key, int scancode, int action, int mods) {

}
void Application::mouse_button_callback(int button, int action, int mods) {

}

void Application::onGUIAnimation() {
  ImGui::Begin("Animation");
  ImGui::Text("Current Time: %.3fs", animation.time);
  ImGui::Checkbox("Interrupted", &(animation.interrupted));
  if (ImGui::Button("Start")) {
    animation.start();
  }
  ImGui::SameLine();
  if (ImGui::Button("Pause")) {
    animation.pause();
  }
  ImGui::SameLine();
  if (ImGui::Button("Stop")) {
    animation.stop();
  }
  ImGui::End();
}

void glfw_error_callback(const int error, const char* description) {
  LOG(ERROR) 
    << "[ERROR]: GLFW error "
    << error 
    << ": " 
    << description 
    << " \n";
}

void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
  app->framebuffer_size_callback(width, height);
  if (app->func_framebuffer_size_callback != nullptr) {
    app->func_framebuffer_size_callback(width, height);
  }
}
void glfw_mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
  app->mouse_callback(static_cast<float>(xpos), static_cast<float>(ypos));
  if (app->func_mouse_callback != nullptr) {
    app->func_mouse_callback(static_cast<float>(xpos), static_cast<float>(ypos));
  }
}
void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
  app->scroll_callback(static_cast<float>(xoffset), static_cast<float>(yoffset));
  if (app->func_scroll_callback != nullptr) {
    app->func_scroll_callback(static_cast<float>(xoffset), static_cast<float>(yoffset));
  }
}
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
  app->key_callback(key, scancode, action, mods);
  if (app->func_key_callback != nullptr) {
    app->func_key_callback(key, scancode, action, mods);
  }
}
void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
  app->mouse_button_callback(button, action, mods);
  if (app->func_mouse_button_callback != nullptr) {
    app->func_mouse_button_callback(button, action, mods);
  }
}

} // namespace middle
} // namespace zexz