#include "application.h"

#include <GLFW/glfw3.h>
#include "nvg.h"

Application::Application(int argc, char** argv):
	window_(glfwCreateWindow( 300, 300, "2048", NULL, NULL),glfwDestroyWindow){

	// Set callback functions
	glfwSetKeyCallback(window_.get(), Application::keyEvent);
	glfwSetFramebufferSizeCallback(window_.get(),Application::resizeWindow);

	int width, height;
	glfwGetFramebufferSize(window_.get(), &width, &height);
	resizeWindow(window_.get(), width, height);

	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwMakeContextCurrent(window_.get());
	glfwSwapInterval(0);
}

Application::~Application(){
}

bool Application::isInitialized() const{
	return window_.get() != NULL;
}

int Application::run()
{
	while(!glfwWindowShouldClose(window_.get()) )
	{
		NVGcontext* vgContext = NVG::instance()->context();
		glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
		float pxRatio = 1.f;
		int width, height;
		glfwGetFramebufferSize(window_.get(), &width, &height);
		nvgBeginFrame(vgContext, width, height, pxRatio);

		// TODO
		nvgBeginPath(vgContext);
		nvgRect(vgContext, 100,100, 120,30);
		nvgCircle(vgContext, 120,120, 5);
		nvgPathWinding(vgContext, NVG_HOLE);   // Mark circle as a hole.
		nvgFillColor(vgContext, nvgRGBA(255,192,0,255));
		nvgFill(vgContext);

		nvgEndFrame(vgContext);

		// Swap buffers
		glfwSwapBuffers(window_.get());
		glfwPollEvents();
	}
	return 0;
}

void Application::resizeWindow(GLFWwindow* window, int width, int height){
	glViewport( 0, 0, (GLint) width, (GLint) height );
}

void Application::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
