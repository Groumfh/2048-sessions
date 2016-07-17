# 2048 applications

set(SOURCES
	src/main.cpp
	src/application.cpp
	src/nvg.cpp
	src/glfw.cpp
	src/boardview.cpp)

# *********************************************
# configure
# *********************************************
# configure embedded resources path ----------
set(RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/")
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/resources/resources_path.h.in
	${CMAKE_CURRENT_BINARY_DIR}/resources/resources_path.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/resources)

# *********************************************
# application
# *********************************************
add_executable(2048-sessions ${SOURCES})
target_include_directories(2048-sessions
	PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/resources)
target_link_libraries(2048-sessions
	PUBLIC core glfw nanovg)
