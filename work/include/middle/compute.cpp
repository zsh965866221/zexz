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


} // namespace middle
} // namespace zexz