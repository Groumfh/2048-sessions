#include "application.h"
#include <core/board.h>
#include "boardview.h"

#include <GLFW/glfw3.h>
#include "nvg.h"

#include <resources_path.h>

namespace{

static Application* app = NULL;

}

Application::Application(int argc, char** argv):
	window_(glfwCreateWindow( 300, 300, "2048", NULL, NULL),glfwDestroyWindow),
	board_(new Board(4,4)){


	board_->setSquare(0,0,4);
	board_->setSquare(0,3,8);
	board_->setSquare(1,2,2);
	board_->setSquare(3,2,2);

	assert(app == NULL);
	app = this;

	boardView_.reset(new BoardView(board_.get()));
	// Set callback functions
	glfwSetKeyCallback(window_.get(), Application::keyEvent);
	glfwSetFramebufferSizeCallback(window_.get(),Application::resizeWindow);

	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwMakeContextCurrent(window_.get());
	glfwSwapInterval(0);
}

Application::~Application(){
}

bool Application::isInitialized() const{
	return window_.get() != NULL;
}

bool Application::initGL(){
	NVGcontext* context = NVG::instance()->context();
	int res = nvgCreateFont(context, "sans", RESOURCE_PATH("Roboto-Regular.ttf").c_str());
	if (res == -1) {
		printf("Could not add font regular.\n");
		return false;
	}
	res = nvgCreateFont(context, "sans-bold", RESOURCE_PATH("Roboto-Bold.ttf").c_str());
	if (res == -1) {
		printf("Could not add font bold.\n");
		return false;
	}
	return true;
}

void Application::paint(NVGcontext* context)
{
	float pxRatio = 1.f;
	int width, height;
	glfwGetFramebufferSize(window_.get(), &width, &height);
	nvgBeginFrame(context, width, height, pxRatio);

	Rect region;
	region.x = 20;
	region.y = 20;
	region.height = height-40;
	region.width = width-40;

	boardView_->paint(context,region);

	nvgEndFrame(context);
}

bool Application::propagateKeyEvent(int key, int scancode, int action, int mods){
	return boardView_->keyEvent(key,scancode,action,mods);
}

int Application::run()
{
	while(!glfwWindowShouldClose(window_.get()) )
	{
		NVGcontext* vgContext = NVG::instance()->context();
		glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

		paint(vgContext);

		// Swap buffers
		glfwSwapBuffers(window_.get());
		glfwPollEvents();
	}
	return 0;
}

void Application::resizeWindow(GLFWwindow* window, int width, int height){
	glViewport( 0, 0, (GLint) width, (GLint) height);
	NVGcontext* vgContext = NVG::instance()->context();
	glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	app->paint(vgContext);

	glfwSwapBuffers(window);
}

void Application::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods){

	if (app->propagateKeyEvent(key,scancode,action,mods)) return;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}
