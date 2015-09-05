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

# third parties -------------------------------

find_package(OpenGL REQUIRED)

#gtest
set(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest/include)
if(APPLE)
	set(GTEST_LIBRARIES
		${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest/lib/libgtest_main.a
		${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest/lib/libgtest.a)
elseif(WIN32)

endif(WIN32)
include_directories(${GTEST_INCLUDE_DIRS})

# nanovg
# TODO create a cmake list for .c
set(NANVOG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nanovg/src)
include_directories(${NANVOG_INCLUDE_DIRS})

# glfw
add_subdirectory(third_party/glfw)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include)

# glew
add_definitions(-DGLEW_STATIC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew-1.13.0/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew-1.13.0/lib/Release/x64)

# libaries ----------------------------------

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/include)

# configure embedded resources path ----------
set(RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/resources_path.h.in
	${CMAKE_CURRENT_BINARY_DIR}/resources/resources_path.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/resources)

set(SOURCES
	main.cpp
	application.cpp
	nvg.cpp
	glfw.cpp
	boardview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew-1.13.0/src/glew.c
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/nanovg/src/nanovg.c)

add_executable(2048-sessions ${SOURCES})

target_link_libraries(2048-sessions
	core
	glfw
	${OPENGL_LIBRARIES}
	${GLFW_LIBRARIES}
	${GLEW_LIBRARY})

if(WIN32)
	target_link_libraries (2048-sessions glu32 opengl32)
endif(WIN32)
