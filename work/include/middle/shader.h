#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace zexz {
namespace middle {

enum ShaderType {
  ShaderType_Vertex = GL_VERTEX_SHADER,
  ShaderType_Fragment = GL_FRAGMENT_SHADER,
  ShaderType_Geometry = GL_GEOMETRY_SHADER,
  ShaderType_Compute = GL_COMPUTE_SHADER
};

class Shader {
public:
  Shader(const std::string& source, ShaderType type);
  Shader();
  virtual ~Shader();

public:
  bool complie();

private:
  bool check_compile_error();

public:
  unsigned int ID;
  std::string source;
  ShaderType type;
};

} // namespace middle
} // namespace zexz