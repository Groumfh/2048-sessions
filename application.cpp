#include "application.h"

#include <core/board.h>
#include "boardview.h"
#include "nvg.h"

#include <GLFW/glfw3.h>
#include <fstream>

#include <random>
#include <algorithm>

#include <resources_path.h>

namespace{

static Application* app = NULL;

}

namespace
{
	bool load(Board* board)
	{
		std::ifstream file("save.game", std::ios::in | std::ios::binary);
		if (file.is_open())
		{
			for (int i = 0; i < board->width(); i++) 
			{
				for (int j = 0; j < board->height(); j++) 
				{
					board->setSquare(i, j, file.get());
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
}

class Application::Impl_ : public non_copyable
{
public:

	Impl_();

	std::unique_ptr<GLFWwindow,void(*)(GLFWwindow*)> window_;
	std::unique_ptr<Board> board_;
	std::unique_ptr<BoardView> boardView_;
	std::unique_ptr<Score> m_Score;
	bool isEnd_;
	bool isBeginning_;
	bool isInMenu_;
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
	isInMenu_(true),
	m_Score(new Score())
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

void drawButton(NVGcontext* context, const Rect & rect, const NVGcolor & color)
{
	nvgBeginPath(context);
	nvgFillColor(context, color);
	nvgRoundedRect(context, rect, 8); // Coutour arondie des coins
	nvgFill(context);
	nvgClosePath(context);
}

void drawText(NVGcontext* context, Rect & rect, const std::string text, float & x, float & y)
{
	x = 0;
	y = 0;
	nvgBeginPath(context);
	rect.center(x, y);
	float txtSize = rect.height;
	nvgFontSize(context, txtSize);
	nvgFontFace(context, "sans");
	nvgTextAlign(context, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
	nvgFill(context);
	nvgFillColor(context, nvgRGBA(0, 0, 0, 255));
	nvgText(context, x + 1, y + 1, text.c_str(), NULL);
	nvgFillColor(context, nvgRGBA(0, 0, 0, 255));
	nvgText(context, x, y, text.c_str(), NULL);
}

void Application::Impl_::paintEvent(NVGcontext* context){

	int winWidth, winHeight;
	glfwGetWindowSize(window_.get(), &winWidth, &winHeight);
	int fWidth, fHeight;
	glfwGetFramebufferSize(window_.get(), &fWidth, &fHeight);
	float pxRatio = (float)fWidth / (float)winWidth;
	nvgBeginFrame(context, winWidth, winHeight, pxRatio);

	if (isInMenu_ == true)
	{
		//1ere couche orange
		Rect MenuContour(10.f, 20.f, winWidth - 20.f, winHeight - 30.f);
		drawButton(context, MenuContour, nvgRGBA(255, 140, 0, 255));

		//2e couche menu blanc
		Rect MenuContour2(14.f, 24.f, MenuContour.width - 10.f, MenuContour.height - 10.f);
		drawButton(context, MenuContour2, nvgRGBA(255, 255, 255, 255));

		// 3e couche menu noir
		Rect MenuContour3(18.f, 28.f, MenuContour.width - 18.f, MenuContour.height - 18.f);
		drawButton(context, MenuContour3, nvgRGBA(0, 0, 0, 255));

		//4e couche Menu Blanc Transparent
		Rect MenuContour4(27.f, 40.f, MenuContour.width - 34.f, MenuContour.height - 34.f);
		drawButton(context, MenuContour4, nvgRGBA(255, 255, 255, 80));

		//Contour texte autour texte 2048
		Rect MenuContour2048(60.f, 50.f, MenuContour.width - 100.f, MenuContour.height / 6);
		drawButton(context, MenuContour2048, nvgRGBA(0, 0, 0, 0));

		//Bouton play
		Rect MenuPlayButton(
			MenuContour.width - MenuContour.width / 1.6f,
			MenuContour.height - MenuContour.height / 2,
			MenuContour.width / 3,
			MenuContour.height / 6);
		drawButton(context, MenuPlayButton, nvgRGBA(255, 255, 255, 80));

		//Bouton Quit
		Rect MenuQuitButton(MenuContour.width - MenuContour.width / 1.6f,
			MenuContour.height - MenuContour.height / 6,
			MenuContour.width / 3,
			MenuContour.height / 6);
		drawButton(context, MenuQuitButton, nvgRGBA(255, 255, 255, 80));

		// & display 2048 menu text
		float xTxt = 0;
		float yTxt = 0;
		drawText(context, MenuContour2048, "2048", xTxt, yTxt);

		// & display Play menu text
		drawText(context, MenuPlayButton, "Play", xTxt, yTxt);

		// & display Quit menu text
		drawText(context, MenuQuitButton, "Quit", xTxt, yTxt);

	}
	else
	{
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
		boardView_->paint(context, boardRect);

		// define upper text properties
		std::string text("SCORE : ");
		text.append(m_Score->getValue().c_str());
		float x = 0;
		float y = 0;
		textRect.center(x, y);
		nvgFontSize(context, 20);
		nvgFontFace(context, "sans");
		nvgTextAlign(context, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);

		if (isEnd_)
		{
			// change the color of the board
			nvgBeginPath(context);
			nvgFillColor(context, nvgRGBA(0, 0, 0, 30));
			nvgRect(context, boardMaxRect);
			nvgFill(context);
			nvgClosePath(context);

			// & display the game over
			text.insert(0, "GAME OVER | ");
			nvgBeginPath(context);

			nvgFill(context);
			nvgFillColor(context, nvgRGBA(0, 0, 0, 255));
			nvgText(context, x + 1, y + 1, text.c_str(), NULL);
			nvgFillColor(context, nvgRGBA(200, 20, 20, 255));
			nvgText(context, x, y, text.c_str(), NULL);
		}
		else
		{
			// draw score
			nvgBeginPath(context);

			nvgFill(context);
			nvgFillColor(context, nvgRGBA(0, 0, 0, 255));
			nvgText(context, x + 1, y + 1, text.c_str(), NULL);
			nvgFillColor(context, nvgRGBA(20, 200, 20, 255));
			nvgText(context, x, y, text.c_str(), NULL);
		}
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

			case GLFW_KEY_HOME: isInMenu_ = !isInMenu_; return;
		}
	}
}

void Application::Impl_::pushOnBoard(Board::Direction direction){
	if (isEnd_ || isInMenu_) return;

	Board::Report mpReport = board_->push(direction);

	for each (auto merge in mpReport.merges_)
	{
		m_Score->add(board_->getSquare(merge.second.x, merge.second.y));
		m_Score->add(board_->getSquare(merge.first.x, merge.first.y));
	}

	if (mpReport.changed()){
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

	if(!load(impl_->board_.get()))
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