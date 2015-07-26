# core test

set(SOURCES
	main.cpp
)

add_executable(test_core ${SOURCES})
target_link_libraries(test_core ${GTEST_LIBRARIES} core)

add_test(NAME core COMMAND test_core)
