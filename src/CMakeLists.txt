file(GLOB zexz_utils_include zexz/utils/*.h)
file(GLOB zexz_utils_src zexz/utils/*.cpp)


add_library(zexz STATIC
  ${zexz_utils_include}
  ${zexz_utils_src}
)

target_link_libraries(zexz
  ${OGL}
  gflags
  ghc_filesystem
  glfw
  glm
  glog
  imgui
  imgui_glfw_ogl
  stb
)

target_include_directories(zexz PUBLIC ./)