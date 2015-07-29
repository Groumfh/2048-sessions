#
# 2048-sessions project
#
cmake_minimum_required(VERSION 2.8.8)

project(2048-sessions)

# activate C++ 11 fetaures
list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g")

set(CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_MODULE_PATH})

# third parties -------------------------------

#gtest
# TODO set a find package for gtest and build it in this repository
set(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/include)
set(GTEST_LIBRARIES
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib/libgtest_main.a
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib/libgtest.a)
include_directories(${GTEST_INCLUDE_DIRS})

# nanovg
# TODO create a cmake list for .c
set(NANVOG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nanovg/src)
include_directories(${NANVOG_INCLUDE_DIRS})

# glfw
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
add_subdirectory(third_party/src/glfw)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/src/glfw/include)

# libaries ----------------------------------


include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/include)

set(SOURCES
	main.cpp
	application.cpp
	nvg.cpp
	glfw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/nanovg/src/nanovg.c
)

add_executable(2048-sessions ${SOURCES})
target_link_libraries(2048-sessions core)
target_link_libraries(2048-sessions glfw ${GLFW_LIBRARIES})
