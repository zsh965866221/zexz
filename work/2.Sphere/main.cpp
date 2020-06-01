#include "middle/application.h"
#include "middle/camera_simple.h"
#include "middle/program.h"
#include "middle/shader.h"
#include "middle/texture.h"
#include "middle/utils.h"

namespace zsh = zexz;

class SimpleApplication: public zsh::middle::Application {
private:
  // Struct
  struct UI {
    UI(): 
      Radius(0.0f), 
      Center(glm::vec2(0.0f, 0.0f)) {}
    float Radius;
    glm::vec2 Center;
  };
  struct Data {
    Data():
      program(nullptr),
      texture(0),
      image_width(0),
      image_height(0),
      image_channels(0),
      VAO(0),
      VBO(0),
      IBO(0) {
    }

    std::unique_ptr<zexz::middle::Program> program;
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
    path_image = resource_dir + "/images/grid.jpg";
    path_vertex = resource_dir + "/shaders/Spherize/spherize.vs";
    path_fragment = resource_dir + "/shaders/Spherize/spherize.fs";
    // texture
    data.texture = zexz::middle::load_texture(
      path_image,
      &(data.image_width), 
      &(data.image_height),
      &(data.image_channels));
    // shader
    zexz::middle::Shader shader_vertex(zexz::middle::ReadText(path_vertex), zexz::middle::ShaderType_Vertex);
    shader_vertex.complie();
    zexz::middle::Shader shader_fragment(zexz::middle::ReadText(path_fragment), zexz::middle::ShaderType_Fragment);
    shader_fragment.complie();
    data.program.reset(new zexz::middle::Program());
    data.program->attach(shader_vertex);
    data.program->attach(shader_fragment);
    data.program->link();
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
    ui.Center = glm::vec2((float)data.image_width / 2.0f, (float)data.image_height / 2.0f);

    return true;
  }

  bool onGUI() {
    ImGui::Begin("Shperize");
    ImGui::SliderFloat("Radius", &(ui.Radius), 0.0f, (float)data.image_width / 2.0f);
    ImGui::DragFloat2("Center of Shpere", &(ui.Center[0]));
    ImGui::End();

    // help
    ImGui::Begin("Help");
    ImGui::Text("1. Mouse Scroll for Scale.");
    ImGui::Text("2. Mouse Middle Button for Move.");
    ImGui::End();

    return true;
  }

  bool onDraw() {
    CHECK_NOTNULL(data.program);

    { // draw
      data.program->use();
      data.program->setTexture("texture1", data.texture, 0);

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
      data.program->setMat4("uMVPMatrix", projection * view * model);

      // texture matrix
      glm::mat4 textureMat = glm::mat4(1.0f);
      textureMat = glm::translate(textureMat, glm::vec3(0.5f, 0.5f, 0.0f));
      textureMat = glm::rotate(textureMat, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      textureMat = glm::rotate(textureMat, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      textureMat = glm::translate(textureMat, glm::vec3(-0.5f, -0.5f, 0.0f));
      data.program->setMat4("uTexuvMat1", textureMat);

      data.program->setTexture("uBitmap1", data.texture, 0);

      data.program->setFloat("uTextureWidth1", (float)data.image_width);
      data.program->setFloat("uTextureHeight1", (float)data.image_height);

      // parameter
      data.program->setFloat("uRadius", ui.Radius / data.image_width);
      data.program->setVec2("uCenter", ui.Center / glm::vec2(data.image_width, data.image_height));
  
      glBindVertexArray(data.VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      data.program->unuse();

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

  std::string path_image;
  std::string path_vertex;
  std::string path_fragment;
};

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_alsologtostderr = true;

  SimpleApplication app;
  app.init();
  app.run();
  return 0;
}


