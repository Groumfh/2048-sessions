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

# third party

# TODO set a find package for gtest and build it in this repository
set(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/include)
set(GTEST_LIBRARIES
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib/libgtest_main.a
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib/libgtest.a)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(core)
include_directories(core/include)

set(SOURCES
	main.cpp
)

add_executable(2048-sessions ${SOURCES})
target_link_libraries(2048-sessions core)
