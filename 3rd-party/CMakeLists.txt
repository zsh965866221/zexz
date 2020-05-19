add_subdirectory(filesystem)
add_subdirectory(gflags)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(glog)
add_subdirectory(SDL)

# glad_local
add_subdirectory(glad_local)

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb/)

# imgui
file(GLOB imgui_source imgui/*.cpp)
file(GLOB imgui_header imgui/*.h)
add_library(imgui STATIC
  ${imgui_source}
  ${imgui_header}
)
target_link_libraries(imgui 
  ${OGL}
)
target_include_directories(imgui PUBLIC imgui/)

add_library(imgui_glfw_ogl STATIC
  imgui/examples/imgui_impl_opengl3.h
  imgui/examples/imgui_impl_opengl3.cpp
  imgui/examples/imgui_impl_glfw.h
  imgui/examples/imgui_impl_glfw.cpp
)
target_link_libraries(imgui_glfw_ogl
  imgui
  glad
  glfw
  ${OGL}
)
target_include_directories(imgui_glfw_ogl PUBLIC imgui/examples/)

add_library(imgui_sdl_ogl STATIC
  imgui/examples/imgui_impl_opengl3.h
  imgui/examples/imgui_impl_opengl3.cpp
  imgui/examples/imgui_impl_sdl.h
  imgui/examples/imgui_impl_sdl.cpp
)
target_link_libraries(imgui_sdl_ogl
  imgui
  glad
  SDL2-static
  ${OGL}
)
target_include_directories(imgui_sdl_ogl PUBLIC imgui/examples/)