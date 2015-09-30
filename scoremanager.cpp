#include "scoremanager.h"

#include <core/board.h>
#include <vector>
#include "boardview.h"
#include "nvg.h"

#include <GLFW/glfw3.h>

#include <random>
#include <algorithm>

#include <resources_path.h>

int ScoreManager::calculScore()
{
	int score = 0;
	for (int i = 0; i < board_->width(); i++)
	{
		for (int j = 0; j < board_->height(); j++)
		{
			score += board_->square(i, j);
		}
	}
	return score;
}

ScoreManager::ScoreManager(Board * board)
{
	board_ = board;
}

/*void ScoreManager::triScore(std::vector<int> scores)
{
	std::sort(scores.begin(), scores.end());
}*/
