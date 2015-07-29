#include "glfw.h"

#include <GLFW/glfw3.h>

bool GLFW_::isInitialized() const {
	return isInitialized_;
}

GLFW_::GLFW_():isInitialized_(false){
	if( !glfwInit() ){
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return;
	}
	isInitialized_ = true;
}

GLFW_::~GLFW_(){
	glfwTerminate();
}

SINGLETON_HOLDER_IMPL(GLFW)
