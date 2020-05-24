#pragma once

#include <iostream>
#include <string>

#include "glad/glad.h"
#include "glog/logging.h"

namespace zexz {
namespace middle {

std::string ReadText(const std::string& path);

void check_gl_error(const std::string& sfile, const int sline);

#define CHECK_GL_ERROR() check_gl_error(__FILE__, __LINE__)


} // namespace middle
} // namespace zexz