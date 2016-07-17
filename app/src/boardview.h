#pragma once

#include <base/base.h>

class Board;

struct NVGcontext;

class BoardView
{
public:
	BoardView(Board* board);
	~BoardView();

	void paint(NVGcontext* context, Rect rect);

private:
	class Impl_;
	std::unique_ptr<Impl_> impl_;
};

