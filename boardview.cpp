#include "boardview.h"

#include <nvg.h>
#include <GLFW/glfw3.h>

#include <core/board.h>
#include <random>
#include <map>
#include <algorithm>
#include <string>

namespace{

void drawShadow(NVGcontext* context,Rect rect){

	NVGpaint shadowPaint = nvgBoxGradient(context, rect + Rect(2,2,0,0), 5*2, 10, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
	nvgBeginPath(context);
	nvgRect(context, rect + Rect(-10,-10,20,20));
	nvgRoundedRect(context, rect, 5);
	nvgPathWinding(context, NVG_HOLE);
	nvgFillPaint(context, shadowPaint);
	nvgFill(context);
	nvgClosePath(context);
}

}

class BoardView::Impl_ : public non_copyable
{
/*private:
	float squareWidth;
	float squareHeight;*/


public:

	Board* board_;

	void drawSquare(NVGcontext* context,int xPos, int yPos, Rect rect)
	{
		int squareValue = board_->square(xPos,yPos);

		// draw text
		if (squareValue != 0)
		{
			Rect squareRect = rect;
			squareRect.addMargin(4);
			nvgBeginPath(context);
			nvgRoundedRect(context,squareRect,5);
			nvgFillColor(context, color(squareValue));
			nvgFill(context);
			nvgClosePath(context);

			drawShadow(context,squareRect);

			nvgBeginPath(context);
			float x= 0;
			float y= 0;
			rect.center(x,y);
			nvgFontSize(context, std::min(rect.width/2,rect.height/2));
			nvgFontFace(context, "sans");
			nvgTextAlign(context, NVG_ALIGN_MIDDLE|NVG_ALIGN_CENTER);
			nvgFillColor(context, nvgRGBA(0,0,0,255));
			nvgText(context,x,y,std::to_string(squareValue).c_str(),NULL);
			nvgClosePath(context);
		}

	}

	NVGcolor color(int value){
		std::map<int,NVGcolor>::iterator it = colors_.find(value);
		if (it != colors_.end()){
			return it->second;
		}

		static std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution(50,255);
		NVGcolor color = nvgRGBA(distribution(generator),distribution(generator),distribution(generator),255);
		colors_[value] = color;
		return color;
	}

	std::map<int,NVGcolor> colors_;

};


BoardView::BoardView(Board * board):
	impl_(new Impl_){

	impl_->board_ = board;
}

BoardView::~BoardView()
{
}

void BoardView::paint(NVGcontext* context, Rect rect)
{
	boardRect = rect;
	float boardMargin = 10;
	boardRect.addMargin(boardMargin);

	// draw border of board rect
	nvgBeginPath(context);
	nvgRoundedRect(context,rect,5);
	nvgRoundedRect(context,boardRect, 5);
	nvgPathWinding(context, NVG_HOLE);
	nvgFillColor(context, nvgRGBA(0,192,100,255));
	nvgFill(context);

	// draw his shadow
	drawShadow(context, rect);

	squareWidth = boardRect.width /impl_->board_->width();
	squareHeight = boardRect.height /impl_->board_->height();

	// draw squares
	for (uint32_t i = 0; i < impl_->board_->width(); i++){
		for (uint32_t j = 0; j < impl_->board_->height(); j++){
			impl_->drawSquare(context,i,j,Rect(boardRect.x+ squareWidth*i,boardRect.y+squareHeight*j,squareWidth,squareHeight));
		}
	}
	nvgClosePath(context);
}


bool BoardView::IsPointInSquare(float xpos, float ypos, uint32_t i, uint32_t j)
{
	Rect square = Rect(0, 0, squareWidth, squareHeight);
	square.x = boardRect.x + squareWidth * i;
	square.y = boardRect.y + squareHeight * j;

	if (IS_MOUSE_IN_RECTANGLE(xpos, ypos, square))
	{
		return true;
	}
	return false;
}
