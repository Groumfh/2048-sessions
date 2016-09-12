find_package(OpenGL)

set(CMAKE_DEBUG_POSTFIX "d")

set(glew_SOURCES src/glew.c)

add_definitions(-DGLEW_STATIC -DGLEW_NO_GLU)

add_library(glew STATIC ${glew_SOURCES})
target_link_libraries(glew ${OPENGL_LIBRARIES})
target_include_directories(glew PUBLIC include ${OPENGL_INCLUDE_DIR})
