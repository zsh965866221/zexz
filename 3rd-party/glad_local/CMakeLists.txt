# Please generate your Glad at https://glad.dav1d.de/

add_library(glad STATIC
  src/glad.c
)
target_include_directories(glad PUBLIC
  include/
)
