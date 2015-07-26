# core library

set(INCLUDE_DIRS
	include
)

set(SOURCES
	src/board.cpp
)

#self includes
include_directories(${INCLUDE_DIRS})
add_library(core ${SOURCES})

#add tests
add_subdirectory(test)
