#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <string>
#include "ghc/filesystem.hpp"

namespace fs = ghc::filesystem;

std::string getResourcesDir() {
  fs::path p_exe(_pgmptr);
  std::cout << p_exe.filename() << std::endl;
  return p_exe.parent_path().parent_path().append("resources");
}

int main() {
  std::string dir_resource = getResourcesDir();
  if (fs::exists(dir_resource) != true) {
    fs::create_directory(dir_resource);
  }
  std::cout << dir_resource << std::endl;
  std::string path_image = fs::path(dir_resource).append("images/grid.jpg");
  std::cout << path_image << std::endl;

  // stb image load
  int height, width, channels;
  unsigned char* buffer = stbi_load(path_image.c_str(), &height, &width, &channels, STBI_rgb);
  if (buffer == nullptr) {
    std::cout << "error load image" << std::endl;
  }
  if(!stbi_write_png(
      fs::path(dir_resource).append("images/grid.png").c_str(), 
      height, width, channels, buffer, 0)) {
    std::cout << "write error" << std::endl;
  }

  return 0;
}