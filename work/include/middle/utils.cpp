#include "middle/utils.h"

#include <fstream>
#include <sstream>

#include "glog/logging.h"

namespace zexz {
namespace middle {

std::string ReadText(const std::string& path) {
  std::string text;

  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open
    file.open(path);
    std::stringstream stream;
    // read
    stream << file.rdbuf();
    // close
    file.close();
    // to string
    text = stream.str();
  } catch (std::ifstream::failure& e) {
    LOG(ERROR) << "[ERROR]: Read shader file error " << e.what() << std::endl;
    // stop if exception happened
    CHECK_NE(1, 1);
  }
  return text;
}

void check_gl_error(const std::string& sfile, const int sline) {
  GLenum error = glGetError();
  switch (error) {
  case GL_INVALID_ENUM:
    LOG(ERROR) << "[ERROR]: " << sfile << ":" << sline << ": GL_INVALID_ENUM" << std::endl;
    break;
  case GL_INVALID_VALUE:
    LOG(ERROR) << "[ERROR]: " << sfile << ":" << sline << ":GL_INVALID_VALUE" << std::endl;
    break;
  case GL_INVALID_OPERATION:
    LOG(ERROR) << "[ERROR]: " << sfile << ":" << sline << ":GL_INVALID_OPERATION" << std::endl;
    break;
  case GL_STACK_OVERFLOW:
    LOG(ERROR) << "[ERROR]: " << sfile << ":" << sline << ":GL_STACK_OVERFLOW" << std::endl;
    break;
  case GL_STACK_UNDERFLOW:
    LOG(ERROR) << "[ERROR]: " << sfile << ":" << sline << ":GL_STACK_UNDERFLOW" << std::endl;
    break;
  case GL_OUT_OF_MEMORY:
    LOG(ERROR) << "[ERROR]: " << sfile << ":" << sline << ":GL_OUT_OF_MEMORY" << std::endl;
    break;
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    LOG(ERROR) << "[ERROR]: " << sfile << ":" << sline << ":GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
    break;
  case GL_CONTEXT_LOST:
    LOG(ERROR) << "[ERROR]: " << sfile << ":" << sline << ":GL_CONTEXT_LOST" << std::endl;
    break;
  }
}

} // namespace middle
} // namespace zexz