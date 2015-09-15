#pragma once

#include <core/board.h>
#include <GLFW/glfw3.h>
#include "nvg.h"
#include <map>
#include <time.h>

enum Achieve
{
	ACH_32,
	ACH_64 ,
	ACH_128 ,
	ACH_256 ,
	ACH_512 ,
	ACH_1024 ,
	ACH_2048
};


class Achievement
{
public:
	Achievement();
	~Achievement();

	void CheckValue(Board::ContentValue value, GLFWwindow* window);
	void Popup(int num);
	void PaintEvent(NVGcontext* context, Rect rect);

private:
	int lastNumAchieve;
	clock_t time_ ;
	std::map<Achieve,std::pair<int, bool>> tabAchieve;
	std::map<int, std::string> tabNameAchieve;

};

