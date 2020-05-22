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

} // namespace middle
} // namespace zexz