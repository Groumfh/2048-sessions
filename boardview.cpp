#include "boardview.h"

#include <GLFW/glfw3.h>
#include <nvg.h>
#include <core/board.h>
#include <random>
#include <map>


class BoardView::Impl_ : public non_copyable
{
public:

	Board* board_;

	void drawSquare(NVGcontext* context,int xPos, int yPos, Rect rect)
	{
		int squareValue = board_->square(xPos,yPos);

		// draw text
		if (squareValue != 0)
		{
			nvgBeginPath(context);
			nvgRoundedRect(context,rect.x+4,rect.y+4, rect.width-8,rect.height-8, 5);
			nvgFillColor(context, color(squareValue));
			nvgFill(context);

			nvgBeginPath(context);
			float x= 0;
			float y= 0;
			rect.center(x,y);
			nvgFontSize(context, std::min(rect.width/2,rect.height/2));
			nvgFontFace(context, "sans");
			nvgTextAlign(context, NVG_ALIGN_MIDDLE|NVG_ALIGN_CENTER);
			nvgFillColor(context, nvgRGBA(0,0,0,255));
			nvgText(context,x,y,std::to_string(squareValue).c_str(),NULL);
		}

	}

	NVGcolor color(int value){
		std::map<int,NVGcolor>::iterator it = colors_.find(value);
		if (it != colors_.end())
		{
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
	Rect boardRect = rect + Rect(10,10,-20,-20);

	// draw border of board rect
	nvgBeginPath(context);
	nvgRoundedRect(context,rect,5);
	nvgRoundedRect(context,boardRect, 5);
	nvgPathWinding(context, NVG_HOLE);
	nvgFillColor(context, nvgRGBA(0,192,100,255));
	nvgFill(context);

	// draw his shadow
	NVGpaint shadowPaint = nvgBoxGradient(context, rect + Rect(2,2,0,0), 5*2, 10, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
	nvgBeginPath(context);
	nvgRect(context, rect + Rect(-10,-10,20,20));
	nvgRoundedRect(context, rect, 5);
	nvgPathWinding(context, NVG_HOLE);
	nvgFillPaint(context, shadowPaint);
	nvgFill(context);

	// draw square

	float squareWidth = boardRect.width /impl_->board_->width();
	float squareHeight = boardRect.height /impl_->board_->height();

	// draw board
	for (uint32_t i = 0; i < impl_->board_->width(); i++){
		for (uint32_t j = 0; j < impl_->board_->height(); j++){
			impl_->drawSquare(context,i,j,Rect(boardRect.x+ squareWidth*i,boardRect.y+squareHeight*j,squareWidth,squareHeight));
		}
	}
}

