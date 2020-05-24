#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "middle/buffer.h"
#include "middle/program.h"
#include "middle/shader.h"

namespace zexz {
namespace middle {


class ComputeProgram: public Program {
public:
  ComputeProgram();
  virtual ~ComputeProgram();

public:
  void compute(
    const unsigned int num_groups_x, 
    const unsigned int num_groups_y, 
    const unsigned int num_groups_z
  );

public:
  template<typename T>
  void bindBuffer(
    const GPUBuffer<T>& buffer, 
    unsigned int index
  ) const;

  void bindImageTexture(
    const std::string& name,
    const GLuint unit,
    const GLuint texture,
    const GLint level = 0,
    const GLboolean layered = false,
    const GLint layer = 0,
    const GLenum access = GL_READ_WRITE,
    const GLenum format = GL_RGBA
  ) const;
};


template<typename T>
void ComputeProgram::bindBuffer(
  const GPUBuffer<T>& buffer, 
  unsigned int index
) const {
  buffer.use();
  glBindBufferBase(buffer.type, index, buffer.ID);
  buffer.unuse();
}

} // namespace middle
} // namespace zexz