#pragma once

// header
#include "zexz/utils/utils.h"

// standard
#include "stdlib.h"

// 3rd
#include "ghc/filesystem.hpp"

namespace fs = ghc::filesystem;

std::string getResourcesDir() {
  fs::path p_exe(_pgmptr);
  return p_exe.parent_path().parent_path().append("resources");
}