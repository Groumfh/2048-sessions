#pragma once

#include "base.h"
#include <core/board.h>

class ScoreManager
{
public:

	ScoreManager(Board *board);

	int calculScore();
	//void triScore(std::vector<int> scores_array);

private:

	Board *board_;
	//std::vector<int> scores_array = { 0, 0, 0 };

};