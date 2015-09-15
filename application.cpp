#include "application.h"

#include <core/board.h>
#include "boardview.h"
#include "nvg.h"
#include <achievement.h>
#include <GLFW/glfw3.h>

#include <random>
#include <algorithm>

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
	std::unique_ptr<Achievement> achieve_;
	bool isEnd_;

	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

	void paintEvent(NVGcontext* context);
	void keyEvent(int key, int scancode, int action, int mods);

	void pushOnBoard(Board::Direction direction);
};

Application::Impl_::Impl_():
	window_(glfwCreateWindow( 300, 300, "2048", NULL, NULL),glfwDestroyWindow),
	board_(new Board(4,4)),
	isEnd_(false),
	achieve_(new Achievement())
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

	Rect textRect(0.f,10.f,width,20.f);
	Rect boardMaxRect(0.f,textRect.height + textRect.y,width,height - textRect.height);

	float boardSizeMin = std::min(boardMaxRect.width,boardMaxRect.height);
	Rect boardRect(20.f, 20.f + textRect.height,boardSizeMin-40.f, boardSizeMin-40.f);

	if (boardMaxRect.height > boardMaxRect.width){
		boardRect.move(0,(boardMaxRect.height - boardMaxRect.width)/2.f);
	}else{
		boardRect.move((boardMaxRect.width - boardMaxRect.height)/2.f,0);
	}

	// draw the text rect
	nvgBeginPath(context);
	nvgFillColor(context, nvgRGBA(0,0,0,50));
	nvgRect(context,textRect);
	nvgFill(context);
	nvgClosePath(context);

	// draw the board
	boardView_->paint(context,boardRect);

	Rect achieveRect( 5.f, boardMaxRect.width - 50.f,  100.f,  40.f);
	achieve_->PaintEvent(context, achieveRect);

	if (isEnd_){

		// change the color of the board
		nvgBeginPath(context);
		nvgFillColor(context, nvgRGBA(0,0,0,30));
		nvgRect(context,boardMaxRect);
		nvgFill(context);
		nvgClosePath(context);

		// & display the game over
		std::string text("GAME OVER");
		nvgBeginPath(context);
		float x= 0;
		float y= 0;
		textRect.center(x,y);
		nvgFontSize(context, 20);
		nvgFontFace(context, "sans");
		nvgTextAlign(context, NVG_ALIGN_MIDDLE|NVG_ALIGN_CENTER);
		nvgFill(context);
		nvgFillColor(context, nvgRGBA(0,0,0,255));
		nvgText(context,x+1,y+1,text.c_str(),NULL);
		nvgFillColor(context, nvgRGBA(200,20,20,255));
		nvgText(context,x,y,text.c_str(),NULL);
	}

	nvgEndFrame(context);
}

void Application::Impl_::keyEvent(int key, int scancode, int action, int mods){

	if (action == GLFW_PRESS){
		switch(key){
			case GLFW_KEY_UP: pushOnBoard(Board::UP); return;
			case GLFW_KEY_DOWN: pushOnBoard(Board::DOWN); return;
			case GLFW_KEY_LEFT: pushOnBoard(Board::LEFT); return;
			case GLFW_KEY_RIGHT: pushOnBoard(Board::RIGHT); return;
			case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window_.get(), GL_TRUE); return;
		}
	}
}

void Application::Impl_::pushOnBoard(Board::Direction direction){
	if (isEnd_) return;

	if (board_->push(direction).changed()){
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

	impl_->board_->setSquare(0,0,2);

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

		for (int i = 0; i < impl_->board_->height(); i++)
			for (int j = 0; j < impl_->board_->width(); j++)
				impl_->achieve_->CheckValue(impl_->board_->square(i, j), impl_->window_.get());

		// Swap buffers
		glfwSwapBuffers(impl_->window_.get());
		glfwPollEvents();

		// test if end is occured
		if (!impl_->board_->isMovable() && !impl_->isEnd_){
			impl_->isEnd_ = true;
		}
	}
	return 0;
}


