find_package(OpenGL)

set(CMAKE_DEBUG_POSTFIX "d")

add_library(glew STATIC src/glew.c)
target_link_libraries(glew ${OPENGL_LIBRARIES})
target_include_directories(glew PUBLIC include ${OPENGL_INCLUDE_DIR})
target_compile_definitions(glew PUBLIC -DGLEW_STATIC -DGLEW_NO_GLU)
