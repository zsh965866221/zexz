#include "middle/application.h"
#include "middle/camera_simple.h"
#include "middle/program.h"
#include "middle/shader.h"
#include "middle/texture.h"
#include "middle/utils.h"


#include <iostream>

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
      Program_bufferA(nullptr),
      Program_bufferB(nullptr),
      Program_main(nullptr),
      texture(0),
      image_width(0),
      image_height(0),
      image_channels(0),
      VAO(0),
      VBO(0),
      IBO(0),
      bufferA(0),
      textureA(0),
      bufferB(0),
      textureB(0) {
    }

    std::unique_ptr<zexz::middle::Program> Program_bufferA;
    std::unique_ptr<zexz::middle::Program> Program_bufferB;
    std::unique_ptr<zexz::middle::Program> Program_main;
    GLuint texture;
    int image_width;
    int image_height;
    int image_channels;
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;

    GLuint bufferA;
    GLuint textureA;

    GLuint bufferB;
    GLuint textureB;
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
    std::string path_image = resource_dir + "/images/grid.jpg";
    std::string path_vertex = resource_dir + "/shaders/bloom/bloom.vs";
    std::string path_fragment_bufferA = resource_dir + "/shaders/bloom/bufferA.fs";
    std::string path_fragment_bufferB = resource_dir + "/shaders/bloom/bufferB.fs";
    std::string path_fragment_main = resource_dir + "/shaders/bloom/main.fs";
    // texture
    data.texture = zexz::middle::load_texture(
      path_image,
      &(data.image_width), 
      &(data.image_height),
      &(data.image_channels));
    // shader
    zexz::middle::Shader shader_vertex(
      zexz::middle::ReadText(path_vertex), 
      zexz::middle::ShaderType_Vertex
    );
    shader_vertex.complie();

    zexz::middle::Shader shader_fragment_bufferA(
      zexz::middle::ReadText(path_fragment_bufferA), 
      zexz::middle::ShaderType_Fragment
    );
    shader_fragment_bufferA.complie();

    zexz::middle::Shader shader_fragment_bufferB(
      zexz::middle::ReadText(path_fragment_bufferB), 
      zexz::middle::ShaderType_Fragment
    );
    shader_fragment_bufferB.complie();

    zexz::middle::Shader shader_fragment_main(
      zexz::middle::ReadText(path_fragment_main), 
      zexz::middle::ShaderType_Fragment
    );
    shader_fragment_main.complie();
    // program
    data.Program_bufferA.reset(new zexz::middle::Program());
    data.Program_bufferA->attach(shader_vertex);
    data.Program_bufferA->attach(shader_fragment_bufferA);
    data.Program_bufferA->link();

    data.Program_bufferB.reset(new zexz::middle::Program());
    data.Program_bufferB->attach(shader_vertex);
    data.Program_bufferB->attach(shader_fragment_bufferB);
    data.Program_bufferB->link();

    data.Program_main.reset(new zexz::middle::Program());
    data.Program_main->attach(shader_vertex);
    data.Program_main->attach(shader_fragment_main);
    data.Program_main->link();

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

    // framebuffer A
    glGenFramebuffers(1, &(data.bufferA));
    glBindFramebuffer(GL_FRAMEBUFFER, data.bufferA);
    glGenTextures(1, &(data.textureA));
    glBindTexture(GL_TEXTURE_2D, data.textureA);
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB, 
      data.image_width, data.image_height, 
      0, GL_RGB, GL_UNSIGNED_BYTE, NULL
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, 
      GL_COLOR_ATTACHMENT0, 
      GL_TEXTURE_2D,
      data.textureA,
      0
    );
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG(ERROR) << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // framebuffer B
    glGenFramebuffers(1, &(data.bufferB));
    glBindFramebuffer(GL_FRAMEBUFFER, data.bufferB);
    glGenTextures(1, &(data.textureB));
    glBindTexture(GL_TEXTURE_2D, data.textureB);
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB, 
      data.image_width, data.image_height, 
      0, GL_RGB, GL_UNSIGNED_BYTE, NULL
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, 
      GL_COLOR_ATTACHMENT0, 
      GL_TEXTURE_2D,
      data.textureB,
      0
    );
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG(ERROR) << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // init gui
    ui.Center = glm::vec2((float)data.image_width / 2.0, (float)data.image_height / 2.0);

    return true;
  }

  bool onGUI() {
    ImGui::Begin("Bloom");
    ImGui::Text("Bloom");
    ImGui::End();

    // help
    ImGui::Begin("Help");
    ImGui::Text("1. Mouse Scroll for Scale.");
    ImGui::Text("2. Mouse Middle Button for Move.");
    ImGui::End();

    return true;
  }

  bool onDraw() {
    CHECK_NOTNULL(data.Program_bufferA);
    CHECK_NOTNULL(data.Program_bufferB);
    CHECK_NOTNULL(data.Program_main);

    { // draw bufferA
      glBindFramebuffer(GL_FRAMEBUFFER, data.bufferA);
      glViewport(0, 0, data.image_width, data.image_height);
      glEnable(GL_DEPTH_TEST);
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      data.Program_bufferA->use();
      data.Program_bufferA->setTexture("iChannel0", data.texture, 0);

      data.Program_bufferA->setMat4("model", glm::mat4(1.0f));
      data.Program_bufferA->setMat4("view", glm::mat4(1.0f));
      data.Program_bufferA->setMat4("projection", glm::mat4(1.0f));

      data.Program_bufferA->setVec2("iResolution", glm::vec2(data.image_width, data.image_height));

      glBindVertexArray(data.VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      data.Program_bufferA->unuse();
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    { // draw bufferB
      glBindFramebuffer(GL_FRAMEBUFFER, data.bufferB);
      glViewport(0, 0, data.image_width, data.image_height);
      glEnable(GL_DEPTH_TEST);
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      data.Program_bufferB->use();
      data.Program_bufferB->setTexture("iChannel0", data.textureA, 0);

      data.Program_bufferB->setMat4("model", glm::mat4(1.0f));
      data.Program_bufferB->setMat4("view", glm::mat4(1.0f));
      data.Program_bufferB->setMat4("projection", glm::mat4(1.0f));

      data.Program_bufferB->setVec2("iResolution", glm::vec2(data.image_width, data.image_height));
  
      glBindVertexArray(data.VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      data.Program_bufferB->unuse();
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    { // draw main
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(0, 0, window_width, window_height);
      glEnable(GL_DEPTH_TEST);
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      data.Program_main->use();

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
      data.Program_main->setMat4("model", model);
      data.Program_main->setMat4("view", view);
      data.Program_main->setMat4("projection", projection);

      data.Program_main->setTexture("iChannel0", data.textureA, 0);
      data.Program_main->setTexture("iChannel1", data.textureB, 1);

      data.Program_main->setVec2("iResolution", glm::vec2(window_width, window_height));

      glBindVertexArray(data.VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      data.Program_main->unuse();
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


