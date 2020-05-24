#include "middle/compute.h"

#include "glog/logging.h"

#include "middle/utils.h"

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
  const GLuint unit,
  const GLuint texture,
  const GLint level,
  const GLboolean layered,
  const GLint layer,
  const GLenum access,
  const GLenum format
) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  CHECK_GL_ERROR();
  glBindTexture(GL_TEXTURE_2D, texture);
  glBindImageTexture(
    unit,
    texture,
    level,
    layered,
    layer,
    access,
    format
  );
  CHECK_GL_ERROR();
  glUniform1i(glGetUniformLocation(ID, name.c_str()), unit);
  CHECK_GL_ERROR();
}


} // namespace middle
} // namespace zexz