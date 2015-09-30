#include "menu.h"

#include "drawutils.h"

#include <nvg.h>
#include <GLFW/glfw3.h>

#include <core/board.h>
#include <random>
#include <map>
#include <algorithm>
#include <string>


class Menu::Impl_ : public non_copyable
{
public:

	Board* board_;

};


Menu::Menu(Board * board) :
	impl_(new Impl_) {

	impl_->board_ = board;
}

Menu::~Menu()
{
}

void Menu::paint(NVGcontext* context, Rect rect, std::vector<char*> btnNames, int currentMode)
{
	Rect boardRect = rect;
	float boardMargin = 10;

	boardRect.addMargin(boardMargin);

	// draw border of board rect
	nvgBeginPath(context);
	nvgRoundedRect(context, rect, 5);
	nvgRoundedRect(context, boardRect, 5);
	nvgPathWinding(context, NVG_HOLE);
	nvgFillColor(context, nvgRGBA(0, 192, 100, 255));
	nvgFill(context);
	nvgClosePath(context);

	// draw his shadow
	drawShadow(context, rect);

	// Buttons position/size
	float buttonHeight = boardRect.height/ btnNames.size();
	float buttonPosY = 0.f;

	float buttonWidth = boardRect.width;
	float buttonPosX = 0.f;

	Rect btn;

	
	// draw buttons
	for (uint32_t i = 0; i < btnNames.size();i++)
	{
		btn.height = buttonHeight;
		btn.width = buttonWidth;
		btn.x = buttonPosX;
		btn.y = buttonPosY;

		btn.move(boardRect.x, boardRect.y);
		btn.addMargin(4);

		
		nvgBeginPath(context);
		nvgRoundedRect(context, btn, 5);
		if (i == currentMode)
		{
			nvgFillColor(context, nvgRGBA(0, 150, 255, 255));
		}
		else
		{
			nvgFillColor(context, nvgRGBA(0, 150, 100, 255));
		}
		 //Button color
		nvgFill(context);
		nvgClosePath(context);

		buttonPosY += buttonHeight;

		drawShadow(context, btn);

		nvgBeginPath(context);
		float x = 0.f;
		float y = 0.f;
		btn.center(x, y);
		// Font parameters
		nvgFontSize(context, std::min(btn.width / 2, btn.height / 2));
		nvgFontFace(context, "sans");
		nvgTextAlign(context, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
		nvgFillColor(context, nvgRGBA(0, 0, 0, 255)); // Text color
		nvgText(context, x, y, btnNames.at(i), NULL);
		nvgClosePath(context);
	}
}

