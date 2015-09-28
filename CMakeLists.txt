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
set(NANVOG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nanovg/src)
include_directories(${NANVOG_INCLUDE_DIRS})
set(NANVOG_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/nanovg/src/nanovg.c)

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
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/include)

# *********************************************
# configure
# *********************************************
# configure embedded resources path ----------
set(RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/resources_path.h.in
	${CMAKE_CURRENT_BINARY_DIR}/resources/resources_path.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/resources)

# *********************************************
# 2048 executable
# *********************************************
set(SOURCES
	main.cpp
	application.cpp
	nvg.cpp
	glfw.cpp
	boardview.cpp
	drawutils.cpp
	menu.cpp
	achievement.cpp
	scoremanager.cpp
	${GLEW_SOURCES}
	${NANVOG_SOURCES})

add_executable(2048-sessions ${SOURCES})
target_link_libraries(2048-sessions
	core
	glfw
	${OPENGL_LIBRARIES}
	${GLFW_LIBRARIES})
if(WIN32)
	target_link_libraries(2048-sessions ${GLEW_LIBRARY} glu32 opengl32)
endif(WIN32)

# *********************************************
# Exercises
# *********************************************
add_subdirectory(exercises/ex1)
add_subdirectory(exercises/ex2)
