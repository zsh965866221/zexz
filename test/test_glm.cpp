#include <iostream>

#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"


glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
  glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

int main(int argc, char* argv[]) {
  auto c = camera(1.0, glm::vec2(2.0, 1.0));
  std::cout << glm::to_string(c) << std::endl;
}