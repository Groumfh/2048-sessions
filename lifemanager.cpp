#include "lifemanager.h"
#include <core/board.h>
#include "nvg.h"
#include <sstream>

LifeManager::LifeManager(GLFWwindow* window, BoardView* boardview)
	: window_(window), boardView_(boardview), lives_(3)
{

}

void LifeManager::setBoardView(BoardView* boardview)
{
	boardView_ = boardview;
}

void LifeManager::removeSquareAt(double xpos, double ypos)
{
	if (lives_ <= 0)
	{
		return;
	}

	int x, y;
	if (boardView_->contains(xpos, ypos))
	{
		boardView_->getCoordinates(xpos, ypos, x, y);
		if (boardView_->getBoard()->square(x, y))
		{
			boardView_->getBoard()->setSquare(x, y, 0);
			lives_ -= 1;
		}
		return;
	}
}



void LifeManager::mouseEvent(int button, int action, int mod, double xpos, double ypos)
{
	if (action == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			removeSquareAt(xpos, ypos);
			return;
		}
	}
}

void LifeManager::paint(NVGcontext* context, Rect rect)
{
	Rect textRect(rect);

	//Display number of remaining lives
	std::string text("Lives : "+std::to_string(lives_));
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
	nvgClosePath(context);

}

int LifeManager::getLives() const {
	return lives_;
}