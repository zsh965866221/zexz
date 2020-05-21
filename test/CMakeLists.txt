add_executable(test_gflags_glog test_gflags_glog.cpp)
target_link_libraries(test_gflags_glog
  gflags
  glog
)

add_executable(test_glm test_glm.cpp)
target_link_libraries(test_glm
  glm
)

add_executable(test_glfw test_glfw.cpp)
target_link_libraries(test_glfw
  glfw
  ${OGL}
)

add_executable(test_stb test_stb.cpp)
target_link_libraries(test_stb
  stb
  ghc_filesystem
)

add_executable(test_imgui_glfw_ogl test_imgui.cpp)
target_link_libraries(test_imgui_glfw_ogl
  imgui
  imgui_glfw_ogl
  glad
  glfw
  ${OGL}
)

add_executable(test_imgui_sdl_ogl test_sdl2.cpp)
target_link_libraries(test_imgui_sdl_ogl
  imgui
  imgui_sdl_ogl
  glad
  SDL2-static
  SDL2main
  ${OGL}
)

add_executable(test_zexz test_zexz.cpp)
target_link_libraries(test_zexz
  zexz
)

add_executable(test_application test_application.cpp)
target_link_libraries(test_application
  glad
  gflags
  glm
  glog
  imgui
  imgui_sdl_ogl
  middle
  stb
  SDL2-static
  SDL2main
  zexz
)

add_executable(test_animation test_animation.cpp)
target_link_libraries(test_animation
  middle
  zexz
)