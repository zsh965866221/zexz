#include "middle/application.h"

#include <iostream>

namespace zsh = zexz;

class SimpleApplication: public zsh::middle::Application {
private:
  // Struct
  struct UI {
    UI(): 
      Swivel(0.0f),
      Tilt(0.0f),
      Distance(0.0f),
      Specular(false) {}
      float Swivel;
      float Tilt;
      float Distance;
      bool Specular;
  };
  struct Data {
    Data():
      shader(nullptr),
      texture(0),
      image_width(0),
      image_height(0),
      image_channels(0),
      VAO(0),
      VBO(0),
      IBO(0) {
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

  struct CameraStruct {
    CameraStruct():
      distance(0.0f),
      mousePosition(glm::vec2(0.0f, 0.0f)),
      midPressed(false),
      mouseMidStart(glm::vec2(0.0f, 0.0f)),
      mouseMidOffset(glm::vec2(0.0f, 0.0f)),
      mouseMidOffsetTmp(glm::vec2(0.0f, 0.0f)) {
    }

    float distance;
    glm::vec2 mousePosition;
    bool midPressed;
    glm::vec2 mouseMidStart;
    glm::vec2 mouseMidOffset;
    glm::vec2 mouseMidOffsetTmp;
  };

public:
  SimpleApplication(): Application(
    "Simple",
    1230,
    900
  ) {}

public:
  bool onInit() {
    // args
    std::string path_image = resource_dir + "/images/f.jpg";
    std::string path_vertex = resource_dir + "/shaders/Basic3D/basic3d.vs";
    std::string path_fragment = resource_dir + "/shaders/Basic3D/basic3d.fs";
    // texture
    data.texture = zexz::middle::load_texture(
      path_image,
      &(data.image_width), 
      &(data.image_height),
      &(data.image_channels));
    // shader
    data.shader.reset(new zexz::middle::Shader(
      path_vertex,
      path_fragment
    ));
    // VAO VBO IBO
    float vertices[] = {
      // positions                 // texture coords
       1.0f,  1.0f, 0.0f, 1.0f,    1.0f, 1.0f, // top right
       1.0f, -1.0f, 0.0f, 1.0f,    1.0f, 0.0f, // bottom right
      -1.0f, -1.0f, 0.0f, 1.0f,    0.0f, 0.0f, // bottom left
      -1.0f,  1.0f, 0.0f, 1.0f,    0.0f, 1.0f  // top left 
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // init gui
    0;

    return true;
  }

  bool onGUI() {
    ImGui::Begin("Basic 3D");
    ImGui::SliderFloat("Swivel", &(ui.Swivel), -180.0, 180.0);
    ImGui::SliderFloat("Tilt", &(ui.Tilt), -180.0, 180.0);
    ImGui::DragFloat(
      "Distance to Image", 
      &(ui.Distance)
    );
    ImGui::Checkbox("Specular Highlight", &(ui.Specular));
    ImGui::End();

    // help
    ImGui::Begin("Help");
    ImGui::Text("1. Mouse Scroll for Scale.");
    ImGui::Text("2. Mouse Middle Button for Move.");
    ImGui::End();

    return true;
  }

  bool onDraw() {
    CHECK_NOTNULL(data.shader);

    { // draw main
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(0, 0, window_width, window_height);
      glEnable(GL_DEPTH_TEST);
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      data.shader->use();

      // MVP
      glm::vec3 viewPos = glm::vec3(0.0, 0.0, -2.0);
      glm::mat4 model = glm::mat4(1.0f);
      glm::mat4 view = glm::lookAt(
        viewPos,
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0)
      );
      glm::mat4 projection = glm::mat4(1.0f);
      model = glm::scale(model, glm::vec3(1.0, (float)(data.image_height)/ (float)(data.image_width), 1.0));
      model = glm::rotate(model, animation.timer.time * 1.0f, glm::vec3(0.0, 0.0, 1.0));
      view = glm::translate(view, 
        glm::vec3(
          -cameraStruct.mouseMidOffset.x / (float)window_width, 
          -cameraStruct.mouseMidOffset.y / (float)window_height, 
          -cameraStruct.distance
        )
      );
      projection = glm::perspective(
        glm::radians(45.0f), 
        static_cast<float>(window_width) / static_cast<float>(window_height),
        0.1f, 
        100.0f);
      data.shader->setMat4("uMVPMatrix", projection * view * model);

      // texture matrix
      glm::mat4 textureMat = glm::mat4(1.0f);
      textureMat = glm::translate(textureMat, glm::vec3(0.5f, 0.5f, 0.0f));
      textureMat = glm::rotate(textureMat, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      textureMat = glm::rotate(textureMat, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      textureMat = glm::translate(textureMat, glm::vec3(-0.5f, -0.5f, 0.0f));
      data.shader->setMat4("uTexuvMat1", textureMat);

      data.shader->setTexture("uBitmap1", data.texture, 0);

      data.shader->setFloat("uTextureWidth1", (float)data.image_width);
      data.shader->setFloat("uTextureHeight1", (float)data.image_height);

      // uniform
      data.shader->setFloat("uTilt", ui.Tilt);
      data.shader->setFloat("uSwivel", ui.Swivel);
      data.shader->setFloat("uDistance", ui.Distance);
      data.shader->setBool("uSpecular", ui.Specular);

      glBindVertexArray(data.VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      data.shader->unuse();
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    return true;
  }

  bool onDestroy() {
    glDeleteVertexArrays(1, &(data.VAO));
    glDeleteBuffers(1, &(data.VBO));
    glDeleteBuffers(1, &(data.IBO));

    return true;
  }

  void onMouseMotion(const SDL_Event* event) {
    int x = event->motion.x;
    int y = event->motion.y;

    cameraStruct.mousePosition = glm::vec2(x, y);
    if (cameraStruct.midPressed == true) {
      cameraStruct.mouseMidOffset = cameraStruct.mouseMidOffsetTmp + (cameraStruct.mousePosition - cameraStruct.mouseMidStart);
    }
  }

  void onMouseButton(const SDL_Event* event) {
    if (event->button.button == SDL_BUTTON_MIDDLE) {
      if (event->type == SDL_MOUSEBUTTONDOWN) {
        cameraStruct.midPressed = true;
        cameraStruct.mouseMidStart = cameraStruct.mousePosition;
      } else if (event->type == SDL_MOUSEBUTTONUP) {
        cameraStruct.midPressed = false;
        cameraStruct.mouseMidOffsetTmp = cameraStruct.mouseMidOffset;
        cameraStruct.mouseMidStart = glm::vec2(0.0, 0.0);
      }
    }
  }

  void onKeyButton(const SDL_Event* event) {
    
  }

  void onMouseWheel(const SDL_Event* event) {
    cameraStruct.distance += ((float)event->wheel.y * 0.1f);
  }

  bool onEvent(const SDL_Event* event) {
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
    case SDL_MOUSEWHEEL:
      onMouseWheel(event);
      break;
    default:
      break;
    }
    return true;
  }

private:
  UI ui;
  Data data;
  CameraStruct cameraStruct;
};

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  FLAGS_alsologtostderr = true;

  SimpleApplication app;
  app.init();
  app.run();
  return 0;
}


