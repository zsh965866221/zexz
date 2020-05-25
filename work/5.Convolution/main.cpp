#include "middle/application.h"
#include "middle/buffer.h"
#include "middle/camera_simple.h"
#include "middle/compute.h"
#include "middle/program.h"
#include "middle/shader.h"
#include "middle/texture.h"
#include "middle/utils.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image.h"

#include <iostream>

namespace zsh = zexz;
using namespace zexz;
using namespace zexz::middle;
using namespace zexz::utils;

class SimpleApplication: public zsh::middle::Application {
private:
  // Struct
  struct UI {
    UI(): 
      Swivel(0.0f),
      Tilt(0.0f),
      Distance(0.0f),
      Specular(false),
      sigma(0.0f) {}
      float Swivel;
      float Tilt;
      float Distance;
      bool Specular;

      // compute 
      float sigma;
  };
  struct Data {
    Data():
      program(nullptr),
      program_compute(nullptr),
      texture(0),
      image_width(0),
      image_height(0),
      image_channels(0),
      VAO(0),
      VBO(0),
      IBO(0) {
    }
    std::unique_ptr<zexz::middle::Program> program;
    std::unique_ptr<zexz::middle::ComputeProgram> program_compute;
    GLuint texture;
    int image_width;
    int image_height;
    int image_channels;
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
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
    std::string path_vertex = resource_dir + "/shaders/Convolution/template.vsh";
    std::string path_fragment = resource_dir + "/shaders/Convolution/template.fsh";
    // texture
    data.texture = zexz::middle::load_texture(
      path_image,
      &(data.image_width), 
      &(data.image_height),
      &(data.image_channels));
    CHECK_GL_ERROR();
    // shader
    zexz::middle::Shader shader_vertex(zexz::middle::ReadText(path_vertex), zexz::middle::ShaderType_Vertex);
    shader_vertex.complie();

    zexz::middle::Shader shader_fragment(zexz::middle::ReadText(path_fragment), zexz::middle::ShaderType_Fragment);
    shader_fragment.complie();
    // program
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

    // camera
    camera.reset(new zexz::middle::SimpleCamera(
      glm::vec3(0.0f, 0.0f, -2.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      window_width, 
      window_height
    ));

    // compute shader
    int work_group_count[3];
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &(work_group_count[0]));
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &(work_group_count[1]));
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &(work_group_count[2]));
    std::cout << work_group_count[0] << "," << work_group_count[1] << "," << work_group_count[2] << std::endl;
    
    int work_group_size[3];
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_group_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_group_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_group_size[2]);
    std::cout << work_group_size[0] << "," << work_group_size[1] << "," << work_group_size[2] << std::endl;

    int work_group_invocations;
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_group_invocations);
    std::cout << work_group_invocations << std::endl;

    std::string path_compute = resource_dir + "/shaders/Convolution/convolution.csh";
    zexz::middle::Shader shader_compute(
      zexz::middle::ReadText(path_compute),
      zexz::middle::ShaderType_Compute
    );
    shader_compute.complie();

    data.program_compute.reset(new zexz::middle::ComputeProgram());
    data.program_compute->attach(shader_compute);
    data.program_compute->link();

    // buffer
    int width, height, channels;
    unsigned char* buffer = stbi_load(path_image.c_str(), &width, &height, &channels, STBI_grey);
    channels = 1;
    glm::ivec2 Kernel(3, 3);
    glm::ivec2 ImageSize(width, height);
    glm::ivec2 Channels(channels, 1);
    GPUBuffer<float> InputData(GPUBufferType_SHADER_STORAGE, Channels.x * ImageSize.x * ImageSize.y);
    GPUBuffer<float> OutputData(GPUBufferType_SHADER_STORAGE, Channels.y * ImageSize.x * ImageSize.y);
    GPUBuffer<float> KernelData(GPUBufferType_SHADER_STORAGE, Channels.y * Channels.x * Kernel.x * Kernel.y);
    GPUBuffer<float> BiasData(GPUBufferType_SHADER_STORAGE, Channels.y);

    int Size = width * height * channels;
    InputData.map([&](const auto* self, auto* data) {
      for (int i = 0; i < Size; i++) {
        data[i] = ((float)buffer[i]) / 255.0f;
      }
    });
    // 1 x 1 x 3 x 3
    KernelData.map([&](const auto* self, auto* data) {
      for (int i = 0; i < Kernel.x * Kernel.y; i++) {
        data[i] = 1.0f;
      }
    });

    OutputData.map([&](const auto* self, auto* data) {
      for (int i = 0; i < Kernel.x * Kernel.y; i++) {
        buffer[i] = (unsigned char)(data[i] * 255.0f);
      }
    });
    

    data.program_compute->use();
    data.program_compute->bindBuffer(InputData, 0);
    data.program_compute->bindBuffer(OutputData, 1);
    data.program_compute->bindBuffer(KernelData, 2);
    data.program_compute->bindBuffer(BiasData, 3);

    glUniform2iv(glGetUniformLocation(data.program_compute->ID, "uKernel"), 1, &(Kernel[0]));
    glUniform2iv(glGetUniformLocation(data.program_compute->ID, "uImageSize"), 1, &(ImageSize[0]));
    glUniform2iv(glGetUniformLocation(data.program_compute->ID, "uChannels"), 1, &(Channels[0]));

    data.program_compute->compute(ImageSize.x / 16 + 1, ImageSize.y / 16 + 1, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    data.program_compute->unuse();


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

    ImGui::Begin("Info");
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::End();

    // help
    ImGui::Begin("Help");
    ImGui::Text("1. Mouse Scroll for Scale.");
    ImGui::Text("2. Mouse Middle Button for Move.");
    ImGui::End();

    return true;
  }

  bool onDraw() {
    { // draw
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(0, 0, window_width, window_height);
      glEnable(GL_DEPTH_TEST);
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      data.program->use();
      
      // MVP
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::scale(model, glm::vec3(1.0, (float)(data.image_height)/ (float)(data.image_width), 1.0));
      model = glm::rotate(model, animation.timer.time * 1.0f, glm::vec3(0.0, 0.0, 1.0));

      data.program->setMat4("uMVPMatrix", camera->projection * camera->view * model);

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

      // uniform
      data.program->setFloat("uTilt", ui.Tilt);
      data.program->setFloat("uSwivel", ui.Swivel);
      data.program->setFloat("uDistance", ui.Distance);
      data.program->setBool("uSpecular", ui.Specular);

      glBindVertexArray(data.VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      CHECK_GL_ERROR();
      data.program->unuse();
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

  bool onEvent(const SDL_Event* event) {
    camera->onEvent(event);
    return true;
  }

private:
  UI ui;
  Data data;
  std::unique_ptr<zsh::middle::SimpleCamera> camera;
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
