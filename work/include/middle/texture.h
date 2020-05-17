#pragma once

#include <string>

#include "glad/glad.h"

namespace zexz {
namespace middle {

GLuint load_texture(
  const std::string& path_file, 
  int* width, 
  int* height, 
  int* channels
);

GLuint load_texture(
  const std::string& path_file
);

} // namespace middle
} // namespace zexz