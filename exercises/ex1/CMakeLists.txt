set(SOURCES
	main.cpp
)

add_executable(ex1 ${SOURCES})
target_link_libraries(ex1 ${GTEST_BOTH_LIBRARIES})

add_test(NAME ex1 COMMAND ex1)
