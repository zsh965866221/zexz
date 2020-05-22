#pragma once

#include <functional>

#include "glad/glad.h"
#include "glog/logging.h"

namespace zexz {
namespace middle{

enum GPUBufferType {
  GPUBufferType_SHADER_STORAGE = GL_SHADER_STORAGE_BUFFER,
};

template<typename T>
class GPUBuffer {
public:
  GPUBuffer(const GPUBufferType type);
  GPUBuffer(
    const GPUBufferType type,
    const unsigned int size,
    const GLenum usage = GL_STATIC_DRAW
  );
  GPUBuffer() = delete;
  virtual ~GPUBuffer();

public:
  void map(std::function<void(const GPUBuffer* self, T* data)> func) const;
  bool allocate(
    const unsigned int size,
    GLenum usage = GL_STATIC_DRAW
  );
  void use() const;
  void unuse() const;

public:
  GLuint ID;
  GPUBufferType type;
  unsigned int size;
  bool allocated;
  GLenum usage;
};

template<typename T>
GPUBuffer<T>::GPUBuffer(const GPUBufferType type):
    type(type),
    allocated(false),
    usage(GL_STATIC_DRAW),
    size(0) {
  glGenBuffers(1, &ID);
}

template<typename T>
GPUBuffer<T>::GPUBuffer(
    const GPUBufferType type,
    const unsigned int size,
    const GLenum usage):
      type(type),
      size(size),
      usage(usage) {
  glGenBuffers(1, &ID);
  allocate(size, usage);
}

template<typename T>
GPUBuffer<T>::~GPUBuffer() {
  glDeleteBuffers(1, &ID);
}

template<typename T>
bool GPUBuffer<T>::allocate(
    const unsigned int size_,
    GLenum usage_) {
  usage = usage_;
  size = size_;
  use();
  glBufferData(
    GL_SHADER_STORAGE_BUFFER,
    sizeof(T) * size,
    nullptr,
    usage
  );
  // TODO: opengl state check
  allocated = true;

  // set it to 0
  map([&](const GPUBuffer<T>* self, T* data) {
    memset(data, 0x00, size * sizeof(T));
  });

  unuse();
  return true;
}

template<typename T>
void GPUBuffer<T>::map(std::function<void(const GPUBuffer<T>* self, T* data)> func) const {
  CHECK_EQ(allocated, true);
  use();
  void* data = glMapBuffer(type, GL_READ_WRITE);
  func(this, static_cast<T*>(data));
  glUnmapBuffer(type);
  unuse();
}

template<typename T>
void GPUBuffer<T>::use() const {
  glBindBuffer(type, ID);
}

template<typename T>
void GPUBuffer<T>::unuse() const {
  glBindBuffer(type, 0);
}

} // namespace middle
} // namespace zexz