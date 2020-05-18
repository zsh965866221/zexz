#include "middle/texture.h"

#include "glog/logging.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace zexz {
namespace middle {

GLuint load_texture(
    const std::string& path_file, 
    int* width, 
    int* height, 
    int* channels) {
  CHECK_NOTNULL(width);
  CHECK_NOTNULL(height);
  CHECK_NOTNULL(channels);

  unsigned char* buffer = 
    stbi_load(path_file.c_str(), width, height, channels, STBI_rgb);
  if (buffer == nullptr) {
    LOG(ERROR) << "[ERROR]: cannot load image\n";
    return 0;
  }
  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    *width,
    *height,
    0,
    GL_RGB,
    GL_UNSIGNED_BYTE,
    buffer);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 15);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(buffer);
  return texID;
}

GLuint load_texture(
    const std::string& path_file) {
  int width;
  int height;
  int channels;
  return load_texture(
    path_file, 
    &width,
    &height,
    &channels);
}


} // namespace middle
} // namespace zexz
