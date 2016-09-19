#pragma once
#define IS_MOUSE_IN_RECTANGLE( mX, mY, rect ) ( mX <= ( rect.x + rect.width ) && mX >= ( rect.x - rect.width  )\
                                                && mY <= ( rect.y + rect.height  ) && mY >= ( rect.y - rect.height  ) )
#include <base.h>
class Board;

struct NVGcontext;

class BoardView
{
public:
	BoardView(Board* board);
	~BoardView();

	void paint(NVGcontext* context, Rect rect);

	bool IsPointInSquare(float xpos, float ypos, uint32_t i, uint32_t j);

private:
	class Impl_;
	std::unique_ptr<Impl_> impl_;

	float squareWidth;
	float squareHeight;
	Rect boardRect;
};

