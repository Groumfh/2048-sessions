#pragma once

#include <base.h>

class Board;

class NVGcontext;

class BoardView
{
public:
	BoardView(Board* board);
	~BoardView();

	void paint(NVGcontext* context, Rect rect);

	bool keyEvent(int key, int scancode, int action, int mods);

private:
	class Impl_;
	std::unique_ptr<Impl_> impl_;
};

