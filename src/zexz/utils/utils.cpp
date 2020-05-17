#pragma once

// header
#include "zexz/utils/utils.h"

// standard
#include "stdlib.h"

// 3rd
#include "ghc/filesystem.hpp"

namespace zexz {
namespace utils {

std::string getResourcesDir() {
  namespace fs = ghc::filesystem;
  fs::path p_exe(_pgmptr);
  return p_exe.parent_path().parent_path().append("resources");
}

} // namespace utils
} // namespace zexz