#include "middle/program.h"

#include <fstream>
#include <sstream>
#include <string>

#include "glog/logging.h"

namespace zexz {
namespace middle {

Program::Program() {
  ID = glCreateProgram();
}

Program::~Program() {
  glDeleteProgram(ID);
}

void Program::attach(
    const Shader& shader) {
  glAttachShader(ID, shader.ID);
}

bool Program::link() {
  glLinkProgram(ID);
  return check_link_error();
}

bool Program::check_link_error() {
  GLint success;
  GLchar info[2048];
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 2048, nullptr, info);
    LOG(ERROR) << "[ERROR]: Program Link Error: \n" << info << "\n"; 
  }
  return success == 1 ? true : false;
}

void Program::use() const {
  glUseProgram(ID);
}

void Program::unuse() const {
  glUseProgram(0);
}

void Program::setBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}
void Program::setInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Program::setFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Program::setVec2(const std::string& name, const glm::vec2& value) const {
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &(value[0]));
}
void Program::setVec2(const std::string& name, float x, float y) const {
  glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void Program::setVec3(const std::string& name, const glm::vec3& value) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &(value[0]));
}
void Program::setVec3(const std::string& name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Program::setVec4(const std::string& name, const glm::vec4& value) const {
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &(value[0]));
}
void Program::setVec4(const std::string& name, float x, float y, float z, float w) const {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
void Program::setMat2(const std::string& name, const glm::mat2& mat) const {
  glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &(mat[0][0]));
}
void Program::setMat3(const std::string& name, const glm::mat3& mat) const {
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &(mat[0][0]));
}
void Program::setMat4(const std::string& name, const glm::mat4& mat) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &(mat[0][0]));
}
void Program::setTexture(const std::string& name, const GLuint texture, const unsigned int id) const {
  glActiveTexture(GL_TEXTURE0 + id);
  glBindTexture(GL_TEXTURE_2D, texture);
  glUniform1i(glGetUniformLocation(ID, name.c_str()), id);
}



} // namespace middle
} // namespace zexz