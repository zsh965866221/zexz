# some middle codes, that is not mature enough to be added to zexz.
file(GLOB middle_header include/middle/*.h)
file(GLOB middle_src include/middle/*.cpp)
add_library(middle STATIC
  ${middle_src}
  ${middle_header}
)
target_link_libraries(middle
  glad
  gflags
  glm
  glog
  imgui
  imgui_sdl_ogl
  SDL2-static
  SDL2main
  stb
  zexz
)
target_include_directories(middle PUBLIC include/)

add_subdirectory(0.Template)
add_subdirectory(1.Basic3D)
add_subdirectory(2.Sphere)
add_subdirectory(3.Bloom)
add_subdirectory(4.Compute)