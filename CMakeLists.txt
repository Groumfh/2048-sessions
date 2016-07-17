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

#OpenGL
find_package(OpenGL REQUIRED)

#gtest
set(GTEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# nanovg
set(NANVOG_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/nanovg/src/nanovg.c)
add_library(nanovg STATIC ${NANVOG_SOURCES})
target_include_directories(nanovg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nanovg/src)

# glfw
add_subdirectory(third_party/glfw)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include)

# glew
add_definitions(-DGLEW_STATIC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew-1.13.0/include)
set(GLEW_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew-1.13.0/src/glew.c)
if(WIN32)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew-1.13.0/lib/Release/x64)
endif(WIN32)

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
