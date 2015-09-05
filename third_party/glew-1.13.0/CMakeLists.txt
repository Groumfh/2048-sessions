set(SOURCE
	src/glew.c
)

include_directories(include)

add_library( glew STATIC
	${GLEW_SOURCE}
)

target_link_libraries( glew
	${OPENGL_LIBRARY}
)