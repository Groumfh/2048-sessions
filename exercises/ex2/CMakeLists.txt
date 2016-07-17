set(SOURCES
	main.cpp
)

add_executable(ex2 ${SOURCES})
target_link_libraries(ex2 gtest)

add_test(NAME ex2 COMMAND ex2)
