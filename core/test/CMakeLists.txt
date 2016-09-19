# core test

set(SOURCES
	main.cpp
)

add_executable(test_core ${SOURCES})
target_link_libraries(test_core core gtest)

add_test(NAME core COMMAND test_core)
