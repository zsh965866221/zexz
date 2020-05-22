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
  );
};


template<typename T>
void ComputeProgram::bindBuffer(
  const GPUBuffer<T>& buffer, 
  unsigned int index
) {
  buffer.use();
  glBindBufferBase(buffer.type, index, buffer.ID);
  buffer.unuse();
}

} // namespace middle
} // namespace zexz