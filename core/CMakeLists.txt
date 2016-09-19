# core library

set(SOURCES
	src/board.cpp
)

file(GLOB_RECURSE HEADERS "*.h")
add_library(core STATIC ${SOURCES} ${HEADERS})
target_include_directories(core PUBLIC include)
target_link_libraries(core PUBLIC base)

#add tests
add_subdirectory(test)
