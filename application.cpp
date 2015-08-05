#include "application.h"
#include <core/board.h>
#include "boardview.h"

#include <GLFW/glfw3.h>
#include "nvg.h"

#include <random>

#include <resources_path.h>

namespace{

static Application* app = NULL;

}

class Application::Impl_ : public non_copyable
{
public:

	Impl_();

	std::unique_ptr<GLFWwindow,void(*)(GLFWwindow*)> window_;
	std::unique_ptr<Board> board_;
	std::unique_ptr<BoardView> boardView_;

	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

	void paintEvent(NVGcontext* context);
	void keyEvent(int key, int scancode, int action, int mods);

	void pushOnBoard(Board::Direction direction);
};

Application::Impl_::Impl_():
	window_(glfwCreateWindow( 300, 300, "2048", NULL, NULL),glfwDestroyWindow),
	board_(new Board(4,4))
{
}

void Application::Impl_::resizeCallback(GLFWwindow* window, int width, int height){
	glViewport( 0, 0, (GLint) width, (GLint) height);
	NVGcontext* vgContext = NVG::instance()->context();
	glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	// redraw directly
	app->impl_->paintEvent(vgContext);

	glfwSwapBuffers(window);
}

void Application::Impl_::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods){
	app->impl_->keyEvent(key,scancode,action,mods);
}

void Application::Impl_::paintEvent(NVGcontext* context){
	float pxRatio = 1.f;
	int width, height;
	glfwGetFramebufferSize(window_.get(), &width, &height);
	nvgBeginFrame(context, width, height, pxRatio);

	int sizemin = std::min(width,height);
	Rect region(20,20,sizemin-40, sizemin-40);
	if (width < height){
		region.move(0,(height - width)/2.f);
	}else{
		region.move((width - height)/2.f,0);
	}

	boardView_->paint(context,region);

	nvgEndFrame(context);
}

void Application::Impl_::keyEvent(int key, int scancode, int action, int mods){

	if (action == GLFW_PRESS){
		switch(key){
			case GLFW_KEY_UP: pushOnBoard(Board::TOP); return;
			case GLFW_KEY_DOWN: pushOnBoard(Board::BOTTOM); return;
			case GLFW_KEY_LEFT: pushOnBoard(Board::LEFT); return;
			case GLFW_KEY_RIGHT: pushOnBoard(Board::RIGHT); return;
		}
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window_.get(), GL_TRUE);
	}
}

void Application::Impl_::pushOnBoard(Board::Direction direction){
	if (board_->push(direction)){
		// generate a random square
		std::vector<Board::Pos> squares = board_->emptySquares();
		if (squares.size() != 0){
			static std::default_random_engine generator;
			std::uniform_int_distribution<int> distribution(0,squares.size()-1);
			int index = distribution(generator);
			board_->setSquare(squares[index].x,squares[index].y,std::uniform_int_distribution<int>(0,1)(generator)? 2:4);
		}
	}
}

Application::Application(int argc, char** argv):
	impl_(new Impl_){

	impl_->board_->setSquare(0,0,4);
	impl_->board_->setSquare(0,3,8);
	impl_->board_->setSquare(1,2,2);
	impl_->board_->setSquare(3,2,2);

	assert(app == NULL);
	app = this;

	impl_->boardView_.reset(new BoardView(impl_->board_.get()));
	// Set callback functions
	glfwSetKeyCallback(impl_->window_.get(), Application::Impl_::keyCallBack);
	glfwSetFramebufferSizeCallback(impl_->window_.get(),Application::Impl_::resizeCallback);

	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwMakeContextCurrent(impl_->window_.get());
	glfwSwapInterval(0);
}

Application::~Application(){
}

bool Application::isInitialized() const{
	return impl_->window_.get() != NULL;
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

int Application::run()
{
	while(!glfwWindowShouldClose(impl_->window_.get()) )
	{
		NVGcontext* context = NVG::instance()->context();
		glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

		impl_->paintEvent(context);

		// Swap buffers
		glfwSwapBuffers(impl_->window_.get());
		glfwPollEvents();
	}
	return 0;
}


