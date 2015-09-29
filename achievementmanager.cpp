#include "achievementmanager.h"

#include <GLFW/glfw3.h>
#include <map>
#include <time.h>

enum Achieve
{
	ACH_32,
	ACH_64,
	ACH_128,
	ACH_256,
	ACH_512,
	ACH_1024,
	ACH_2048
};


class AchievementManager::Impl_ : public non_copyable
{
public:
	Board * board_;
	int lastNumAchieve_;
	clock_t time_;
	std::map<int, std::string> tabNameAchieve_;
	std::map<int, std::string> achievementGet_;
};

AchievementManager::AchievementManager(Board *board):
	impl_(new Impl_)
{
	impl_->tabNameAchieve_[32] = " x86 ";
	impl_->tabNameAchieve_[64] = " 64 bits ";
	impl_->tabNameAchieve_[128] = " Almost";
	impl_->tabNameAchieve_[256] = " Colorfull ! ";
	impl_->tabNameAchieve_[512] = " Still here ?";
	impl_->tabNameAchieve_[1024] = " What ?!";
	impl_->tabNameAchieve_[2048] = " !!! ";

	impl_->lastNumAchieve_ = 0;
	impl_->board_ = board;
}


AchievementManager::~AchievementManager()
{
}

void AchievementManager::checkBoard()
{
	for (int i = 0; i <impl_->board_->height(); i++)
		for (int j = 0; j < impl_->board_->width(); j++)
			checkValue(impl_->board_->square(i, j));
}

void AchievementManager::checkValue(Board::ContentValue value)
{
	if (impl_->achievementGet_.count(value) == 0 && value >=32 )
	{
		impl_->achievementGet_[value] = impl_->tabNameAchieve_[value];
		initiatePopup(value);
	}
}

void AchievementManager::initiatePopup(int num)
{
	impl_->lastNumAchieve_ = num;
	impl_->time_ = clock();
}


void AchievementManager::paintEvent(NVGcontext* context, Rect rect)
{
	Rect textRect(rect);
	Rect boardRect = rect;
	float boardMargin = 1;
	boardRect.addMargin(boardMargin);

	clock_t fading = clock()- impl_->time_;

	if (impl_->lastNumAchieve_ > 0)
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

		// & display the Achievement
		std::string text(impl_->tabNameAchieve_[impl_->lastNumAchieve_]);
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
			impl_->lastNumAchieve_ = 0;
	}
}
