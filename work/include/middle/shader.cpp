#include "middle/shader.h"

#include <fstream>
#include <sstream>
#include <string>

#include "glog/logging.h"

namespace zexz {
namespace middle {

Shader::Shader(
    const std::string& path_vertex,
    const std::string& path_fragment,
    const std::string& path_geometry) {

  std::string vertexCode;
  std::string fragmentCode;
  std::string geometryCode;

  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  std::ifstream gShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    // open
    vShaderFile.open(path_vertex);
    fShaderFile.open(path_fragment);
    std::stringstream vstream, fstream;
    // read
    vstream << vShaderFile.rdbuf();
    fstream << fShaderFile.rdbuf();
    // close
    vShaderFile.close();
    fShaderFile.close();
    // to string
    vertexCode = vstream.str();
    fragmentCode = fstream.str();

    if (path_geometry != "") {
      gShaderFile.open(path_geometry);
      std::stringstream gstream;
      gstream << gShaderFile.rdbuf();
      gShaderFile.close();
      geometryCode = gstream.str();
    }
  } catch (std::ifstream::failure& e) {
    LOG(ERROR) << "[ERROR]: Read shader file error " << e.what() << std::endl;
    // stop if exception happened
    CHECK_NE(1, 1);
  }
  const char* vcode = vertexCode.c_str();
  const char* fcode = fragmentCode.c_str();
  // compile
  unsigned int vertex, fragment;
  bool ret;
  // vertex
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vcode, nullptr);
  glCompileShader(vertex);
  ret = check_compile_errors(vertex, ShaderCompileErrorType_Shader_Vertex);
  CHECK_EQ(ret, true);
  // fragment
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fcode, nullptr);
  glCompileShader(fragment);
  ret = check_compile_errors(fragment, ShaderCompileErrorType_Shader_Fragment);
  unsigned int geometry = 0;
  if (path_geometry != "") {
    const char* gcode = geometryCode.c_str();
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gcode, nullptr);
    glCompileShader(geometry);
    ret = check_compile_errors(geometry, ShaderCompileErrorType_Shader_Geometry);
    CHECK_EQ(ret, true);
  }
  // program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  if (path_geometry != "") {
    glAttachShader(ID, geometry);
  }
  glLinkProgram(ID);
  ret = check_compile_errors(ID, ShaderCompileErrorType_Program);

  // delete
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (path_geometry != "") {
    glDeleteShader(geometry);
  }
}

Shader::~Shader() {
  glDeleteProgram(ID);
}

bool Shader::check_compile_errors(
    GLuint shader, 
    ShaderCompileErrorType type) {
  GLint success;
  GLchar info[2048];
  if (type == ShaderCompileErrorType_Program) {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 2048, nullptr, info);
      LOG(ERROR) << "[ERROR]: Program Link Error: \n" << info << "\n"; 
    }
  } else {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 2048, nullptr, info);
      std::string sshader;
      if (type == ShaderCompileErrorType_Shader_Vertex) {
        sshader = "Vertex";
      } else if (type == ShaderCompileErrorType_Shader_Fragment) {
        sshader = "Fragment";
      } else {
        sshader = "Geometry";
      }
      LOG(ERROR) << "[ERROR]: Shader " << sshader << " Compile Error: \n" << info << "\n"; 
    }
  }
  return success == 1 ? true : false;
}

void Shader::use() const {
  glUseProgram(ID);
}

void Shader::unuse() const {
  glUseProgram(0);
}

void Shader::setBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}
void Shader::setInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &(value[0]));
}
void Shader::setVec2(const std::string& name, float x, float y) const {
  glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &(value[0]));
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &(value[0]));
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const {
  glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &(mat[0][0]));
}
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const {
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &(mat[0][0]));
}
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &(mat[0][0]));
}
void Shader::setTexture(const std::string& name, const GLuint texture, const unsigned int id) const {
  glActiveTexture(GL_TEXTURE0 + id);
  glBindTexture(GL_TEXTURE_2D, texture);
  glUniform1i(glGetUniformLocation(ID, name.c_str()), id);
}



} // namespace middle
} // namespace zexz