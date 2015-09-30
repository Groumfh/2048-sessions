#pragma once
#include "base.h"
#include <random>


class Board;
struct NVGcontext;

class Menu {
public:
	Menu(Board * board_);
	~Menu();

	void paint(NVGcontext* context, Rect rect, std::vector<char*> btnNames, int currentMode = 0);
private:
	class Impl_;
	std::unique_ptr<Impl_> impl_;
};