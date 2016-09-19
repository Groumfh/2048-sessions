#pragma once
#define IS_MOUSE_IN_RECTANGLE( mX, mY, rect ) ( mX <= ( rect.x + rect.width / 2.0f ) && mX >= ( rect.x - rect.width / 2.0f )\
                                                && mY <= ( rect.y + rect.height / 2.0f ) && mY >= ( rect.y - rect.height / 2.0f ) )
#include <base.h>
class Board;

struct NVGcontext;

class BoardView
{
public:
	BoardView(Board* board);
	~BoardView();

	void paint(NVGcontext* context, Rect rect);

	bool IsPointInSquare(float xpos, float ypos,uint32_t i, uint32_t j, int height, int width)
	{
		Rect square = Rect(0,0 , squareWidth, squareHeight);
		square.x =  width- boardRect.width-10+ squareWidth*i;
		square.y = height- boardRect.height-10+ squareHeight*j;
		if (IS_MOUSE_IN_RECTANGLE(xpos, ypos, square))
		{
			return true;
		}
		return false;
	}

private:
	class Impl_;
	std::unique_ptr<Impl_> impl_;

	float squareWidth;
	float squareHeight;
	Rect boardRect;
};

