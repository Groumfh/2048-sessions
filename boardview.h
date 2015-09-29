#pragma once

#include <base.h>

class Board;

struct NVGcontext;

class BoardView
{
public:
	BoardView(Board* board);
	~BoardView();
	enum modeEnum { numeric, symboles, smiley, alphabet, romain ,COUNT};
	void setMode(modeEnum m);

	modeEnum getMode();
	modeEnum getNextMode();

	void paint(NVGcontext* context, Rect rect);
	Board* getBoard() const;

	bool contains(double xpos, double ypos);

	void getCoordinates(double xpos, double ypos, int& x, int& y);

private:
	class Impl_;
	std::unique_ptr<Impl_> impl_;
};

