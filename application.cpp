#include "application.h"
#include "scoremanager.h"

#include <core/board.h>
#include "boardview.h"
#include "menu.h"
#include "nvg.h"
#include <achievement.h>
#include "lifemanager.h"
#include <GLFW/glfw3.h>

#include <random>
#include <algorithm>
#include <sstream>

#include <resources_path.h>

namespace {

	static Application* app = NULL;
	enum AppState
	{
		MainMenu,
		Play,
		Mode,
		HallOfFame,
		End
	};

}

class Application::Impl_ : public non_copyable
{
public:

	Impl_();

	std::unique_ptr<GLFWwindow,void(*)(GLFWwindow*)> window_;
	std::unique_ptr<Board> board_;
	std::unique_ptr<Menu> menu_;
	std::unique_ptr<BoardView> boardView_;
	std::unique_ptr<Achievement> achieve_;
	std::unique_ptr<ScoreManager> scoreManager_;
	std::unique_ptr<LifeManager> lifeManager_;

	AppState AS;

	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	void paintEvent(NVGcontext* context);
	void keyEvent(int key, int scancode, int action, int mods);

	void pushOnBoard(Board::Direction direction);
};

Application::Impl_::Impl_():
	window_(glfwCreateWindow( 300, 300, "2048", NULL, NULL),glfwDestroyWindow),
	board_(new Board(4,4)),
	achieve_(new Achievement(board_.get())),
	lifeManager_(new LifeManager(window_.get(), nullptr)),
	AS(MainMenu)
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

void Application::Impl_::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	app->impl_->lifeManager_.get()->mouseEvent(button, action, mods, xpos, ypos);
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

	Rect livesRect(width - 60.f, 10.f, 45.f, 20);
	if (AS == MainMenu)
	{
		//Start graphic
		menu_->paint(context, boardRect, std::vector<char*>{ "1. Play", "2. Mode", "3. Hall of fame", "4. Quit"});
	}

	if (AS == Mode)
	{
		//Mode graphic
		menu_->paint(context, boardRect, std::vector<char*>{ "1. Numeric", "2. Symboles", "3. Smiley", "4. Alphabet", "5. Romains", "6. Jouer" }, boardView_->getMode());
	}

	if (AS == HallOfFame)
	{
		//Hall of Fame graphic
		menu_->paint(context, boardRect, std::vector<char*>{ "1.", "2.", "3.", "4.", "5.", "6.","btn2"});
	}

	if (AS == Play)
	{
		// draw the board
		boardView_->paint(context, boardRect);
		lifeManager_->paint(context, livesRect);
	}

	if (AS == End) {

		// change the color of the board
		nvgBeginPath(context);
		nvgFillColor(context, nvgRGBA(0,0,0,30));
		nvgRect(context,boardMaxRect);
		nvgFill(context);
		nvgClosePath(context);

		// & display the game over
		int score = scoreManager_->calculScore();
		std::stringstream ss;
		ss << score;
		std::string str = ss.str();
		std::string text("GAME OVER     SCORE " + str);
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

	Rect achieveRect(5.f, boardMaxRect.width - 50.f, 100.f, 40.f);
	achieve_->PaintEvent(context, achieveRect);

	nvgEndFrame(context);
}

void Application::Impl_::keyEvent(int key, int scancode, int action, int mods){

	if (action == GLFW_PRESS) {
		if (AS == Play)
		{
			switch (key)
			{
				case GLFW_KEY_UP: pushOnBoard(Board::UP); return;
				case GLFW_KEY_DOWN: pushOnBoard(Board::DOWN); return;
				case GLFW_KEY_LEFT: pushOnBoard(Board::LEFT); return;
				case GLFW_KEY_RIGHT: pushOnBoard(Board::RIGHT); return;
				case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window_.get(), GL_TRUE); return;
				case GLFW_KEY_SPACE: {
					int i = int(boardView_->getMode());
					i++;
					if (BoardView::modeEnum(i) == BoardView::COUNT) {
						i = 0;
					}
					boardView_->setMode(BoardView::modeEnum(i));
					return;
				}
			}
		}

		if (AS == MainMenu)
		{
			switch (key)
			{
				case GLFW_KEY_KP_1: AS = Play; return;
				case GLFW_KEY_KP_2: AS = Mode; return;
				case GLFW_KEY_KP_3: AS = HallOfFame; return;
				case GLFW_KEY_KP_4: glfwSetWindowShouldClose(window_.get(), GL_TRUE); return;
			}
		}

		if (AS == Mode)
		{
			switch (key)
			{
				case GLFW_KEY_KP_1: boardView_->setMode(BoardView::numeric); return;
				case GLFW_KEY_KP_2: boardView_->setMode(BoardView::symboles); return;
				case GLFW_KEY_KP_3: boardView_->setMode(BoardView::smiley); return;
				case GLFW_KEY_KP_4: boardView_->setMode(BoardView::alphabet); return;
				case GLFW_KEY_KP_5: boardView_->setMode(BoardView::romain); return;
				case GLFW_KEY_KP_6: AS = Play; return;
			}
		}

		if (AS == HallOfFame)
		{
			switch (key)
			{
				case GLFW_KEY_KP_1: AS = Play; return;
				case GLFW_KEY_KP_2: glfwSetWindowShouldClose(window_.get(), GL_TRUE); return;
			}
		}
	}
}

void Application::Impl_::pushOnBoard(Board::Direction direction){
	if (AS==End) return;

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


Application::Application(int argc, char** argv) :
	impl_(new Impl_) {
	impl_->board_->setSquare(0, 0, 2);

	assert(app == NULL);
	app = this;

	impl_->boardView_.reset(new BoardView(impl_->board_.get()));
	impl_->scoreManager_.reset(new ScoreManager(impl_->board_.get()));

	impl_->lifeManager_->setBoardView(impl_->boardView_.get());

	// Set callback functions
	glfwSetKeyCallback(impl_->window_.get(), Application::Impl_::keyCallBack);
	glfwSetMouseButtonCallback(impl_->window_.get(), Application::Impl_::mouseButtonCallback);
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
	while (!glfwWindowShouldClose(impl_->window_.get()))
	{
		NVGcontext* context = NVG::instance()->context();
		glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		impl_->paintEvent(context);

		impl_->achieve_->CheckBoard();

		// Swap buffers
		glfwSwapBuffers(impl_->window_.get());
		glfwPollEvents();

		// test if end is occured
		if (!impl_->board_->isMovable() && impl_->AS!=End && impl_->lifeManager_->getLives()<=0) {
			impl_->AS=End;
		}
	}
	return 0;
}





