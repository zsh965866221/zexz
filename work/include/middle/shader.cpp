#include "middle/shader.h"

#include <fstream>
#include <sstream>
#include <string>

#include "glog/logging.h"

namespace zexz {
namespace middle {

static std::string GetShaderTypeName(ShaderType type) {
  switch (type) {
  case ShaderType_Compute:
    return "Compute";
  case ShaderType_Fragment:
    return "Fragment";
  case ShaderType_Geometry:
    return "Geometry";
  case ShaderType_Vertex:
    return "Vertex";
  }
}

Shader::Shader(
  const std::string& source,
  ShaderType type):
    source(source),
    type(type) {
  ID = glCreateShader(type);
}

Shader::Shader():
    Shader("", ShaderType_Vertex) {}

bool Shader::complie() {
  const char* ssource = source.c_str();
  glShaderSource(ID, 1, &ssource, nullptr);
  glCompileShader(ID);
  return check_compile_error();
}

Shader::~Shader() {
  glDeleteShader(ID);
}

bool Shader::check_compile_error() {
  GLint success;
  GLchar info[2048];
  glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(ID, 2048, nullptr, info);
    LOG(ERROR) << "[ERROR]: Shader " << GetShaderTypeName(type) << " Compile Error: \n" << info << "\n"; 
  }
  return success == 1 ? true : false;
}

} // namespace middle
} // namespace zexz