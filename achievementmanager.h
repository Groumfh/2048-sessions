#pragma once

#include <core/board.h>
#include "nvg.h"


class AchievementManager
{
public:
	AchievementManager(Board * board);
	~AchievementManager();

	void checkBoard();
	void checkValue(Board::ContentValue value);
	void initiatePopup(int num);
	void paintEvent(NVGcontext* context, Rect rect);

private:
	class Impl_;
	std::unique_ptr<Impl_> impl_;

};

