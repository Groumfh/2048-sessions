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
public:

	Board* board_;

	Rect cachedRect_;

	void drawSquare(NVGcontext* context,int xPos, int yPos, Rect rect)
	{
		int squareValue = board_->square(xPos, yPos);

		if (squareValue != 0)
		{
			Rect squareRect = rect;
			squareRect.addMargin(4);
			nvgBeginPath(context);
			nvgRoundedRect(context, squareRect, 5);
			nvgFillColor(context, color(squareValue));
			nvgFill(context);
			nvgClosePath(context);

			drawShadow(context, squareRect);

			nvgBeginPath(context);
			float x = 0;
			float y = 0;
			rect.center(x, y);
			nvgFontSize(context, std::min(rect.width / 2, rect.height / 2));
			nvgFontFace(context, "sans");
			nvgTextAlign(context, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
			nvgFillColor(context, nvgRGBA(0, 0, 0, 255));
			if (mode == numeric) {
				nvgText(context,x,y,std::to_string(squareValue).c_str(),NULL);
			}
			else {
				nvgText(context, x, y, modeMap[mode][squareValue], NULL);
			}
			nvgClosePath(context);
		}

	}

	NVGcolor color(int value) {
		std::map<int, NVGcolor>::iterator it = colors_.find(value);
		if (it != colors_.end()) {
			return it->second;
		}

		static std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution(50, 255);
		NVGcolor color = nvgRGBA(distribution(generator), distribution(generator), distribution(generator), 255);
		colors_[value] = color;
		return color;
	}

	std::map<int, NVGcolor> colors_;

	modeEnum mode = romain;

	std::map<int, const char*> dicSymbole = { { 2,"&" },{ 4,"~" },{ 8,"{}" },{ 16,"[#]" },{ 32,"|--|" },{ 64,"`\`" },{ 128,"@@" },{ 256,"°" },{ 512,"=£=" },{ 1024,"$$$" },{ 2048,"!!!µ%¤%!!!" } };
	std::map<int, const char*> dicSmiley = { { 2,"x(" },{ 4,"--'" },{ 8,":(" },{ 16,":|" },{ 32,":p" },{ 64,":)" },{ 128,";)" },{ 256,"8)" },{ 512,"^^" },{ 1024,"XD" },{ 2048,":D" } };
	std::map<int, const char*> dicAlphaB = { { 2,"A" },{ 4,"B" },{ 8,"C" },{ 16,"D" },{ 32,"E" },{ 64,"F" },{ 128,"G" },{ 256,"H)" },{ 512,"I" },{ 1024,"J" },{ 2048,"K" } };
	std::map<int, const char*> dicRomain = { { 2,"II" },{ 4,"IV" },{ 8,"VIII" },{ 16,"XVI" },{ 32,"XXXII" },{ 64,"LXIV" },{ 128,"CXXVIII" },{ 256,"CCLVI)" },{ 512,"DXII" },{ 1024,"MXXIV" },{ 2048,"MMXLVIII" } };
	std::map<modeEnum, std::map<int, const char*>> modeMap = { { symboles,dicSymbole }, { smiley,dicSmiley },{ alphabet,dicAlphaB },{ romain,dicRomain } };
};




BoardView::BoardView(Board * board):
	impl_(new Impl_){

	impl_->board_ = board;
}

BoardView::~BoardView()
{
}

void BoardView::setMode(modeEnum m) {
	impl_->mode = m;
}

BoardView::modeEnum BoardView::getMode() {
	return impl_->mode;
}

BoardView::modeEnum BoardView::getNextMode() {
	int i = int(impl_->mode);
	i++;
	if (modeEnum(i) == COUNT) {
		i++;
	}
	return modeEnum(i);
}

void BoardView::paint(NVGcontext* context, Rect rect)
{
	Rect boardRect = rect;
	float boardMargin = 10;
	boardRect.addMargin(boardMargin);
	impl_->cachedRect_ = boardRect;

	// draw border of board rect
	nvgBeginPath(context);
	nvgRoundedRect(context,rect,5);
	nvgRoundedRect(context,boardRect, 5);
	nvgPathWinding(context, NVG_HOLE);
	nvgFillColor(context, nvgRGBA(0,192,100,255));
	nvgFill(context);

	// draw his shadow
	drawShadow(context, rect);

	float squareWidth = boardRect.width /impl_->board_->width();
	float squareHeight = boardRect.height /impl_->board_->height();

	// draw squares
	for (uint32_t i = 0; i < impl_->board_->width(); i++){
		for (uint32_t j = 0; j < impl_->board_->height(); j++){
			impl_->drawSquare(context,i,j,Rect(boardRect.x+ squareWidth*i,boardRect.y+squareHeight*j,squareWidth,squareHeight));
		}
	}
	nvgClosePath(context);

}

Board* BoardView::getBoard() const{
	return impl_->board_;
}

bool BoardView::contains(double xpos, double ypos){
	Rect boardRect = impl_->cachedRect_;
	//If point at (xpos,ypos) is contained within boardRect
	return (xpos > boardRect.x && xpos<boardRect.x + boardRect.width && ypos>boardRect.y && ypos < boardRect.y + boardRect.height);
}

void BoardView::getCoordinates(double xpos, double ypos, int& x, int& y){
	assert(contains(xpos,ypos));
	Rect boardRect = impl_->cachedRect_;
	float squareWidth = boardRect.width / impl_->board_->width();
	float squareHeight = boardRect.height / impl_->board_->height();
	x = (int)((xpos - boardRect.x) / squareWidth);
	y = (int)((ypos - boardRect.y) / squareHeight);
}
