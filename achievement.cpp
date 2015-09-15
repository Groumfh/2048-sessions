#include "achievement.h"



Achievement::Achievement():
	lastNumAchieve(0)
{
	tabAchieve[Achieve::ACH_32] = std::make_pair(32, false);
	tabAchieve[Achieve::ACH_64]   = std::make_pair(64, false);
	tabAchieve[Achieve::ACH_128]  = std::make_pair(128, false);
	tabAchieve[Achieve::ACH_256]  = std::make_pair(256, false);
	tabAchieve[Achieve::ACH_512]  = std::make_pair(512, false);
	tabAchieve[Achieve::ACH_1024] = std::make_pair(1024, false);
	tabAchieve[Achieve::ACH_2048] = std::make_pair(2048, false);

	tabNameAchieve[32] = "32 bits";
	tabNameAchieve[64] = "64 bits";
	tabNameAchieve[128] = " Almost";
	tabNameAchieve[256] = " Colorfull ! ";
	tabNameAchieve[512] = " Still here ?";
	tabNameAchieve[1024] = " What ?!";
	tabNameAchieve[2048] = " !!! ";
}


Achievement::~Achievement()
{
}

void Achievement::CheckValue(Board::ContentValue value)
{
	for (std::map<Achieve, std::pair<int, bool>>::iterator it = tabAchieve.begin() ; it != tabAchieve.end() ; it++)
		if (value == it->second.first && !it->second.second)
		{
			it->second.second = true;
			Popup(it->second.first);
		}
}

void Achievement::Popup(int num)
{
	lastNumAchieve = num;
	time_ = clock();
	//std::cout << "Hurra ! tu as atteins " << num << std::endl;
}


void Achievement::PaintEvent(NVGcontext* context, Rect rect)
{
	Rect textRect(rect);
	Rect boardRect = rect;
	float boardMargin = 1;
	boardRect.addMargin(boardMargin);

	clock_t fading = clock()-time_;

	if (lastNumAchieve > 0)
	{

		// draw the background
		nvgBeginPath(context);
		nvgFillColor(context, nvgRGBA(255, 255, 255, 50));
		nvgRoundedRect(context, rect,5);
		nvgFill(context);
		nvgClosePath(context);

		// draw border of rect
		nvgBeginPath(context);
		nvgRoundedRect(context, rect, 5);
		nvgRoundedRect(context, boardRect, 5);
		nvgPathWinding(context, NVG_HOLE);
		nvgFillColor(context, nvgRGBA(255, 0, 0, 255));
		nvgFill(context);

		// & display the game over
		std::string text(tabNameAchieve[lastNumAchieve]);
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

		if (fading >=3000)
			lastNumAchieve = 0;
	}
}
