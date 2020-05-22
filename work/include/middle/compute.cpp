#include "middle/compute.h"

#include "glog/logging.h"

namespace zexz {
namespace middle {

ComputeProgram::ComputeProgram() {}

ComputeProgram::~ComputeProgram() {}

void ComputeProgram::compute(
    const unsigned int num_groups_x, 
    const unsigned int num_groups_y, 
    const unsigned int num_groups_z) {
  glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

void ComputeProgram::bindImageTexture(
  const std::string& name,
  const GLuint texture,
  const GLint level,
  const GLboolean layered,
  const GLint layer,
  const GLenum access,
  const GLenum format
) const {
  GLuint location = glGetUniformLocation(ID, name.c_str());
  glBindImageTexture(
    location,
    texture,
    level,
    layered,
    layer,
    access,
    format
  );
}

void ComputeProgram::bindImageTexture(
  const GLuint unit,
  const GLuint texture,
  const GLint level,
  const GLboolean layered,
  const GLint layer,
  const GLenum access,
  const GLenum format
) const {
  glBindImageTexture(
    unit,
    texture,
    level,
    layered,
    layer,
    access,
    format
  );
}

} // namespace middle
} // namespace zexz