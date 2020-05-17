#include <string>
#include <memory>

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

static void glfw_error_callback(int error, const char* description);
static void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void glfw_mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void glfw_keyboard_callback(GLFWwindow* window);

// setting
static unsigned int SCREEN_WIDTH = 1280;
static unsigned int SCREEN_HEIGHT = 720;
static glm::vec4 BACKGROUND_COLOR(0.45f, 0.55f, 0.60f, 1.00f);

// on
static void onInit();
static void onGUI();
static void onDraw();
static void onDestory();

// Struct
struct UI {
  float rotate_x;
  float rotate_y;
  float distance;
};
struct Data {
  Data() {
    shader.reset(nullptr);
  }

  std::unique_ptr<zexz::middle::Shader> shader;
  GLuint texture;
  int image_width;
  int image_height;
  int image_channels;
  GLuint VAO;
  GLuint VBO;
  GLuint IBO;
};

// static
static UI ui;
static Data data;
static std::string PATH_RESOURCE = zexz::utils::getResourcesDir();

// args
DEFINE_string(image, PATH_RESOURCE + "/images/grid.jpg", "image");
DEFINE_string(vertex, PATH_RESOURCE + "/shaders/basic3d.vs", "vertex");
DEFINE_string(fragment, PATH_RESOURCE + "/shaders/basic3d.fs", "fragment");

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_alsologtostderr = true;

  // init glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* window = glfwCreateWindow(
    SCREEN_WIDTH, SCREEN_HEIGHT, "Basic3D", nullptr, nullptr);
  if (window == nullptr) {
    LOG(ERROR) << "[ERROR]: Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
  glfwSetErrorCallback(glfw_error_callback);
  glfwSetCursorPosCallback(window, glfw_mouse_callback);
  glfwSetScrollCallback(window, glfw_scroll_callback);

  // load glad opengl interface
  if (!gladLoadGL()) {
    LOG(ERROR) << "[ERROR]: Failed to init glad opengl interface" << std::endl;
    glfwTerminate();
    return -1;
  }

  // init IMGUI
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  // style
  ImGui::StyleColorsDark();
  // render
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 300 es");

  // init custom
  onInit();

  // gl
  glEnable(GL_DEPTH_TEST);

  // loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glfw_keyboard_callback(window);

    // ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    onGUI();
    ImGui::Render();
    
    glClearColor(
      BACKGROUND_COLOR.r,
      BACKGROUND_COLOR.g,
      BACKGROUND_COLOR.b, 
      BACKGROUND_COLOR.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw
    onDraw();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // swap buffer
    glfwSwapBuffers(window);
  }

  // clean up
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  onDestory();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
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
  SCREEN_HEIGHT = height;
  SCREEN_WIDTH = width;
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void glfw_mouse_callback(GLFWwindow* window, double xpos, double ypos) {

}
void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

}
void glfw_keyboard_callback(GLFWwindow* window) {

}

void onInit() {
  // texture
  data.texture = zexz::middle::load_texture(
    FLAGS_image,
    &(data.image_width), 
    &(data.image_height),
    &(data.image_channels));
  // shader
  data.shader.reset(new zexz::middle::Shader(
    FLAGS_vertex,
    FLAGS_fragment
  ));
  // VAO VBO IBO
  float vertices[] = {
    // positions           // texture coords
     1.0f,  1.0f, 0.0f,    1.0f, 1.0f, // top right
     1.0f, -1.0f, 0.0f,    1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f,    0.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f,    0.0f, 1.0f  // top left 
  };
  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
  };
  glGenVertexArrays(1, &(data.VAO));
  glGenBuffers(1, &(data.VBO));
  glGenBuffers(1, &(data.IBO));

  glBindVertexArray(data.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // texture coord
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void onGUI() {
  ImGui::Begin("Basic 3D");
  ImGui::SliderFloat("Rotate X: ", &(ui.rotate_x), 0.0, 360.0);
  ImGui::SliderFloat("Rotate Y: ", &(ui.rotate_y), 0.0, 360.0);
  ImGui::SliderFloat("Distance: ", &(ui.distance), -3.0, 3.0);
  ImGui::End();
}

void onDraw() {
  data.shader->use();
  data.shader->setTexture("texture1", data.texture, 0);

  // basic 3d
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::lookAt(
    glm::vec3(0.0, 0.0, -3.0),
    glm::vec3(0.0, 0.0, 0.0),
    glm::vec3(0.0, 1.0, 0.0)
  );
  glm::mat4 projection = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(ui.rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(ui.rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, ui.distance));
  projection = glm::perspective(
    glm::radians(45.0f), 
    static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
    0.1f, 
    100.0f);
  data.shader->setMat4("MVP", projection * view * model);

  glBindVertexArray(data.VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  data.shader->unuse();
}

void onDestory() {
  glDeleteVertexArrays(1, &(data.VAO));
  glDeleteBuffers(1, &(data.VBO));
  glDeleteBuffers(1, &(data.IBO));
}
