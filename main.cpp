#include "application.h"
#include "GL/glew.h"
#include "nvg.h"
#include "glfw.h"

int main(int argc, char **argv){

	GLFW::start();
	if (!GLFW::instance()->isInitialized()){
		exit(-1);
	}

	Application app(argc,argv);
	if (!app.isInitialized()){
		exit(-1);
	}

	glewInit();

	NVG::start();
	if (!NVG::instance()->isInitialized()){
		exit(-1);
	}
	if (!app.initGL()){
		exit(-1);
	}

	int result = app.run();

	NVG::stop();
	GLFW::stop();

	return result; // 1011
}

