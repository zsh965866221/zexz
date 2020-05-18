#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace zexz {
namespace middle {

enum ShaderCompileErrorType {
  ShaderCompileErrorType_Shader_Vertex = 0,
  ShaderCompileErrorType_Shader_Fragment,
  ShaderCompileErrorType_Shader_Geometry,
  ShaderCompileErrorType_Program
};

class Shader {
public:
  Shader(
    const std::string& path_vertex,
    const std::string& path_fragment,
    const std::string& path_geometry = "");
  virtual ~Shader();

public:
  void use() const;
  void unuse() const;
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setVec2(const std::string& name, const glm::vec2& value) const;
  void setVec2(const std::string& name, float x, float y) const;
  void setVec3(const std::string& name, const glm::vec3& value) const;
  void setVec3(const std::string& name, float x, float y, float z) const;
  void setVec4(const std::string& name, const glm::vec4& value) const;
  void setVec4(const std::string& name, float x, float y, float z, float w) const;
  void setMat2(const std::string& name, const glm::mat2& mat) const;
  void setMat3(const std::string& name, const glm::mat3& mat) const;
  void setMat4(const std::string& name, const glm::mat4& mat) const;
  void setTexture(const std::string& name, const GLuint texture, const unsigned int id) const;
  


private:
  bool check_compile_errors(
    GLuint shader, 
    ShaderCompileErrorType type);

public:
  unsigned int ID;
};

} // namespace middle
} // namespace zexz