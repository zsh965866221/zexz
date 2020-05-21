#include "middle/application.h"

#include <fstream>
#include <sstream>
#include <string>

#include "glog/logging.h"

namespace zexz {
namespace middle {

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
      func_onEvent(nullptr),
      func_onGUI(nullptr),
      func_onDraw(nullptr),
      func_onDestroy(nullptr) {
  resource_dir = zexz::utils::getResourcesDir();
}

Application::Application(): 
    Application(
      "zexz",
      1230,
      960
    ) {}

Application::~Application() {
  cleanIMGUI();
  onDestroy();
  if (func_onDestroy != nullptr) {
    func_onDestroy(this);
  }
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
bool Application::initSDL() {
  if (SDL_Init(
        SDL_INIT_VIDEO | 
        SDL_INIT_AUDIO |
        SDL_INIT_TIMER |
        SDL_INIT_GAMECONTROLLER) != 0) {
    LOG(ERROR) << "[ERROR]: SDL init ERROR: " << SDL_GetError() << std::endl;
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version_major);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, version_minor);

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_WindowFlags window_flags = 
    (SDL_WindowFlags)(
      SDL_WINDOW_OPENGL | 
      SDL_WINDOW_RESIZABLE | 
      SDL_WINDOW_ALLOW_HIGHDPI);
  window = SDL_CreateWindow(
    window_name.c_str(), 
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, 
    window_width,
    window_height,  
    window_flags);
  gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  // load glad opengl interface
  if (!gladLoadGL()) {
    LOG(ERROR) << "[ERROR]: Failed to init glad opengl interface" << std::endl;
    SDL_Quit();
    return false;
  }
  return true;
}

bool Application::initIMGUI() {
  // init IMGUI
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  // style
  ImGui::StyleColorsDark();
  // render
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 300 es");
  return true;
}

bool Application::init() {
  bool ret;
  ret = initSDL();
  CHECK_EQ(ret, true);
  ret = initIMGUI();
  CHECK_EQ(ret, true);

  // init
  onInit();
  if (func_onInit != nullptr) {
    func_onInit(this);
  }

  return ret;
}

bool Application::run() {
  glEnable(GL_DEPTH_TEST);
  bool running = true;
  while (running == true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      onEvent(&event);
      if (func_onEvent != nullptr) {
        func_onEvent(this, &event);
      }

      if (event.type == SDL_QUIT) {
        running = false;
      }
      if (event.type == SDL_WINDOWEVENT && 
          event.window.event == SDL_WINDOWEVENT_CLOSE && 
          event.window.windowID == SDL_GetWindowID(window)) {
        running = false;
      }
    }

    // timer and frame
    time = timer.delta();
    frame++;
    animation.timer.update();

    // update
    onUpdate();
    if (func_onUpdate != nullptr) {
      func_onUpdate(this);
    }

    //ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();
    onGUIAnimation();
    onGUI();
    if(func_onGUI != nullptr) {
      func_onGUI(this);
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
      func_onDraw(this);
    }
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // swap buffer
    SDL_GL_SwapWindow(window);
  }
  return true;
}

bool Application::cleanIMGUI() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
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

bool Application::onDestroy() {
  return true;
}

bool Application::onUpdate() {
  return true;
}

bool Application::onEvent(const SDL_Event* event) {
  return true;
}

void Application::onGUIAnimation() {
  ImGui::Begin("Animation");
  ImGui::Text("Current Time: %.3fs", animation.timer.time);
  ImGui::Checkbox("Interrupted", &(animation.timer.interrupted));
  if (ImGui::Button("Start")) {
    animation.timer.start();
  }
  ImGui::SameLine();
  if (ImGui::Button("Pause")) {
    animation.timer.pause();
  }
  ImGui::SameLine();
  if (ImGui::Button("Stop")) {
    animation.timer.stop();
  }
  ImGui::End();
}

void Application::onInit(std::function<bool(Application* self)> func) {
  func_onInit = func;
}

void Application::onEvent(std::function<bool(Application* self, const SDL_Event* event)> func) {
  func_onEvent = func;
}

void Application::onGUI(std::function<bool(Application* self)> func) {
  func_onGUI = func;
}

void Application::onDraw(std::function<bool(Application* self)> func) {
  func_onDraw = func;
}

void Application::onDestroy(std::function<bool(Application* self)> func) {
  func_onDestroy = func;
}

void Application::onUpdate(std::function<bool(Application* self)> func) {
  func_onUpdate = func;
}

} // namespace middle
} // namespace zexz