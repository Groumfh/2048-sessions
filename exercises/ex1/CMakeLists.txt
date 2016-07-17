set(SOURCES
	main.cpp
)

add_executable(ex1 ${SOURCES})
target_link_libraries(ex1 gtest)

add_test(NAME ex1 COMMAND ex1)
