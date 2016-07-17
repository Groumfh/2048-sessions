# core library

set(SOURCES
	src/board.cpp
)

add_library(core STATIC ${SOURCES})
target_include_directories(core PUBLIC include)
target_link_libraries(core PUBLIC base)

#add tests
add_subdirectory(test)
