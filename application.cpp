#include "application.h"

#include <core/board.h>
#include "boardview.h"
#include "nvg.h"

#include <GLFW/glfw3.h>

#include <random>
#include <algorithm>

#include <resources_path.h>
#include "Button.h"

#include<iostream>

namespace {

	static Application* app = NULL;

}

class Application::Impl_ : public non_copyable
{

private:
	std::vector<Button*> buttons;
public:

	Impl_();

	std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> window_;
	std::unique_ptr<Board> board_;
	std::unique_ptr<BoardView> boardView_;
	bool isEnd_;

	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void clickCallBack(GLFWwindow* window, int button, int action, int mods);
	void paintEvent(NVGcontext* context);
	void keyEvent(int key, int scancode, int action, int mods);
	void clickEvent(int button, int action, int mods);

	void pushOnBoard(Board::Direction direction);


	Rect boardRect;
};




void Application::Impl_::clickCallBack(GLFWwindow* window, int button, int action, int mods)
{
	app->impl_->clickEvent(button, action, mods);

}

Application::Impl_::Impl_() :
	window_(glfwCreateWindow(300, 300, "2048", NULL, NULL), glfwDestroyWindow),
	board_(new Board(4, 4)),
	isEnd_(false)
{
}

void Application::Impl_::resizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, (GLint)width, (GLint)height);
	NVGcontext* vgContext = NVG::instance()->context();
	glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// redraw directly
	app->impl_->paintEvent(vgContext);

	glfwSwapBuffers(window);
}

void Application::Impl_::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	app->impl_->keyEvent(key, scancode, action, mods);
}

void Application::Impl_::paintEvent(NVGcontext* context) {

	int winWidth, winHeight;
	glfwGetWindowSize(window_.get(), &winWidth, &winHeight);
	int fWidth, fHeight;
	glfwGetFramebufferSize(window_.get(), &fWidth, &fHeight);
	float pxRatio = (float)fWidth / (float)winWidth;
	nvgBeginFrame(context, winWidth, winHeight, pxRatio);

	Rect textRect(0.f, 10.f, winWidth, 20.f);
	Rect boardMaxRect(0.f, textRect.height + textRect.y, winWidth, winHeight - textRect.height);

	float boardSizeMin = std::min(boardMaxRect.width, boardMaxRect.height);
	Rect boardRect(20.f, 20.f + textRect.height, boardSizeMin - 40.f, boardSizeMin - 40.f);

	if (boardMaxRect.height > boardMaxRect.width) {
		boardRect.move(0, (boardMaxRect.height - boardMaxRect.width) / 2.f);
	}
	else {
		boardRect.move((boardMaxRect.width - boardMaxRect.height) / 2.f, 0);
	}

	// draw the text rect
	nvgBeginPath(context);
	nvgFillColor(context, nvgRGBA(0, 0, 0, 50));
	nvgRect(context, textRect);
	nvgFill(context);
	nvgClosePath(context);

	// draw the board
	this->boardRect = boardRect;
	boardView_->paint(context, boardRect);

	if (isEnd_) {

		// change the color of the board
		nvgBeginPath(context);
		nvgFillColor(context, nvgRGBA(0, 0, 0, 30));
		nvgRect(context, boardMaxRect);
		nvgFill(context);
		nvgClosePath(context);

		// & display the game over
		std::string text("GAME OVER");
		nvgBeginPath(context);
		float x = 0;
		float y = 0;
		textRect.center(x, y);
		nvgFontSize(context, 20);
		nvgFontFace(context, "sans");
		nvgTextAlign(context, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
		nvgFill(context);
		nvgFillColor(context, nvgRGBA(0, 0, 0, 255));
		nvgText(context, x + 1, y + 1, text.c_str(), NULL);
		nvgFillColor(context, nvgRGBA(200, 20, 20, 255));
		nvgText(context, x, y, text.c_str(), NULL);

		constexpr char* retryLabel = "RECOMMENCER";

		nvgBeginPath(context);

		float xRecommencer = fWidth / 2.0f,
			yRecommencer = fHeight / 2.0f;

		textRect.center(x, y);
		nvgFontSize(context, 20);
		nvgFontFace(context, "sans");
		nvgTextAlign(context, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
		nvgFill(context);
		nvgFillColor(context, nvgRGBA(0, 0, 0, 255));
		nvgText(context, xRecommencer + 1, yRecommencer + 1, retryLabel, nullptr);
		nvgFillColor(context, nvgRGBA(200, 20, 20, 255));
		nvgText(context, xRecommencer, yRecommencer, retryLabel, nullptr);

		constexpr char* exitLabel = "QUITTER";

		nvgBeginPath(context);

		float xLabel = fWidth / 2.0f,
			yLabel = fHeight / 2.0f + 20.0f;

		textRect.center(x, y);
		nvgFontSize(context, 20);
		nvgFontFace(context, "sans");
		nvgTextAlign(context, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
		nvgFill(context);
		nvgFillColor(context, nvgRGBA(0, 0, 0, 255));
		nvgText(context, xLabel + 1, yLabel + 1, exitLabel, nullptr);
		nvgFillColor(context, nvgRGBA(200, 20, 20, 255));
		nvgText(context, xLabel, yLabel, exitLabel, nullptr);
	}

	nvgEndFrame(context);
}

void Application::Impl_::keyEvent(int key, int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_UP: pushOnBoard(Board::UP); return;
		case GLFW_KEY_DOWN: pushOnBoard(Board::DOWN); return;
		case GLFW_KEY_LEFT: pushOnBoard(Board::LEFT); return;
		case GLFW_KEY_RIGHT: pushOnBoard(Board::RIGHT); return;
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window_.get(), GL_TRUE); return;

			// DEBUG KEYS
		case GLFW_KEY_F1: isEnd_ = true; // F1: forces game over screen
		}
	}
}


void Application::Impl_::clickEvent(int button, int action, int mods)
{
	double xpos = 0.0,
		ypos = 0.0;
	glfwGetCursorPos(window_.get(), &xpos, &ypos);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		for each (Button* label_button in buttons)
		{
			if (IS_MOUSE_IN_RECTANGLE(xpos, ypos, label_button->GetRectangle()))
			{
				switch (label_button->GetButtonType())
				{
				case B_play:
					//action à exec quand bouton jouer clické
					break;
				case B_exit:
					//exit app
					break;
				case B_retry:
					//recommencer jeu
					break;
				default:
					break;
				}
				return;
			}
		}
			for (uint32_t i = 0; i < board_.get()->width(); i++)
			{
				for (uint32_t j = 0; j < board_->height(); j++)
				{
					if (board_->square(i, j) != NULL)
					{
						int *h = new int();
						int *w = new int();
						glfwGetWindowSize(window_.get(), w, h);
						if (boardView_->IsPointInSquare(xpos,ypos,i,j,*h,*w))
						{
							//int val = board_->square(i, j);
							//std::cout << val;
						}
					}
					
				}
			}
		}
}

void Application::Impl_::pushOnBoard(Board::Direction direction) {
	if (isEnd_) return;

	if (board_->push(direction).changed()) {
		// generate a random square
		std::vector<Board::Pos> squares = board_->emptySquares();
		if (squares.size() != 0) {
			static std::default_random_engine generator;
			std::uniform_int_distribution<int> distribution(0, squares.size() - 1);
			int index = distribution(generator);
			board_->setSquare(squares[index].x, squares[index].y, std::uniform_int_distribution<int>(0, 1)(generator) ? 2 : 4);
		}
	}
}


Application::Application(int argc, char** argv) :
	impl_(new Impl_) {

	impl_->board_->setSquare(0, 0, 2);

	assert(app == NULL);
	app = this;

	impl_->boardView_.reset(new BoardView(impl_->board_.get()));
	// Set callback functions
	glfwSetKeyCallback(impl_->window_.get(), Application::Impl_::keyCallBack);
	glfwSetFramebufferSizeCallback(impl_->window_.get(), Application::Impl_::resizeCallback);

	glfwSetMouseButtonCallback(impl_->window_.get(), Application::Impl_::clickCallBack);

	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwMakeContextCurrent(impl_->window_.get());
	glfwSwapInterval(0);
}

Application::~Application() {
}

bool Application::isInitialized() const {
	return impl_->window_.get() != NULL;
}

bool Application::initGL() {
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
	while (!glfwWindowShouldClose(impl_->window_.get()))
	{
		NVGcontext* context = NVG::instance()->context();
		glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		impl_->paintEvent(context);

		// Swap buffers
		glfwSwapBuffers(impl_->window_.get());
		glfwPollEvents();

		// test if end is occured
		if (!impl_->board_->isMovable() && !impl_->isEnd_) {
			impl_->isEnd_ = true;
		}
	}
	return 0;
}
