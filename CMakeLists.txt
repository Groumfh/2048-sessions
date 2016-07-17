#
# 2048-sessions project
#
cmake_minimum_required(VERSION 2.8.8)

project(2048-sessions)

# activate C++ 11 features
list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g")

set(CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_MODULE_PATH})

include(ExternalProject)

# *********************************************
# third parties
# *********************************************

# OpenGL
find_package(OpenGL REQUIRED)
add_library(opengl INTERFACE)
target_link_libraries(opengl INTERFACE ${OPENGL_LIBRARIES})
target_include_directories(opengl INTERFACE ${OPENGL_INCLUDE_DIR})

#gtest
set(GTEST_ROOT third_party/googletest/googletest)
add_library(gtest ${GTEST_ROOT}/src/gtest-all.cc)
target_include_directories(gtest PUBLIC ${GTEST_ROOT} ${GTEST_ROOT}/include)

# nanovg
add_library(nanovg STATIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nanovg/src/nanovg.c)
target_include_directories(nanovg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nanovg/src)
target_link_libraries(nanovg PRIVATE opengl)

# glfw
add_subdirectory(third_party/glfw)

# *********************************************
# libaries
# *********************************************
add_subdirectory(base)
add_subdirectory(core)
add_subdirectory(app)

# *********************************************
# Exercises
# *********************************************
add_subdirectory(exercises/ex1)
add_subdirectory(exercises/ex2)
