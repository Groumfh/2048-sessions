#include "core/board.h"

#include <vector>
#include <exception>
#include <assert.h>

#include <base.h>

namespace{

enum Axe{
	AXE_VERTICAL = Board::DOWN|Board::UP,
	AXE_HORIZONTAL = Board::LEFT|Board::RIGHT
};

typedef std::vector<uint32_t> Array;

template <class Type>
bool mergeCondition(Type lValue, Type rValue, Type & res){
	if (lValue == rValue){
		res = lValue + rValue;
		return true;
	}
	return false;
}

template <class ForwardIt>
void pushArray(ForwardIt itbegin, ForwardIt itEnd, bool& hasChanged){
	ForwardIt empty = itEnd;
	ForwardIt lastValue = itEnd;
	ForwardIt it = itbegin;
	for(; it != itEnd; it++){
		// if it == 0, save the last empty position
		if (*it == 0){
			if (empty == itEnd) empty = it;
		}
		else {
			typename ForwardIt::value_type res;
			if (lastValue != itEnd && mergeCondition(*it,*lastValue,res)){
				*lastValue = res;
				*it = 0;
				it = lastValue;
				hasChanged = true;
			}
			else if (empty != itEnd){
				*empty = *it;
				*it = 0;
				it = empty;
				empty = itEnd;
				hasChanged = true;
			}
		}
		// if it != 0, save the last value position
		if (*it != 0){
			lastValue = it;
		}
	}
}
}

#define ENSURE_ACCESS_VALID(impl,x,y) \
	if (x >= impl->width_ || y >= impl->height_)\
		throw std::out_of_range("bad access to a square of the board");

class Board::Impl_ : public non_copyable{
public:

	Impl_();
	std::vector<std::vector<uint32_t> > values_;

	uint32_t width_;
	uint32_t height_;

	void init(uint32_t width, uint32_t height);

	Array getArray(Axe axe, uint32_t index) const;
	void setArray(Axe axe, uint32_t index, Array& data);

	bool testAndPush(Board::Direction direction, bool apply = true);
};

Board::Impl_::Impl_():
	width_(0),
	height_(0){
}

void Board::Impl_::init(uint32_t width, uint32_t height){
	height_ = height;
	width_ = width;
	values_.resize(width);
	for (std::vector<uint32_t>& subValue : values_) {
		subValue.resize(height,0u);
	}
}

Array Board::Impl_::getArray(Axe axe, uint32_t index) const{
	if (axe == AXE_VERTICAL){
		ENSURE_ACCESS_VALID(this,index,0);
		Array data (height_);
		for (uint32_t i = 0; i < height_; i++)
			data[i] = values_[index][i];
		return data;
	}
	else{
		ENSURE_ACCESS_VALID(this,0,index);
		Array data (width_);
		for (uint32_t i = 0; i < width_; i++)
			data[i] = values_[i][index];
		return data;
	}
}

void Board::Impl_::setArray(Axe axe, uint32_t index, Array& data)
{
	if (axe == AXE_VERTICAL){
		assert(data.size() == height_);
		for (uint32_t i = 0; i < height_; i++)
			values_[index][i] = data[i];
	}
	else{
		assert(data.size() == width_);
		for (uint32_t i = 0; i < width_; i++)
			values_[i][index] = data[i];
	}
}

bool Board::Impl_::testAndPush(Board::Direction direction,bool apply)
{
	bool stateChanged = false;
	if (direction & AXE_HORIZONTAL && width_ > 1){
		for (int i = 0; i < height_; i++){
			Array array = getArray(AXE_HORIZONTAL,i);
			if (direction == Board::LEFT){
				pushArray(array.begin(),array.end(),stateChanged);
			}
			else {
				pushArray(array.rbegin(),array.rend(),stateChanged);
			}
			if (apply){
				setArray(AXE_HORIZONTAL,i,array);
			}
		}
	}
	else if (direction & AXE_VERTICAL && height_ > 1){
		for (int i = 0;i < width_; i++){
			Array array = getArray(AXE_VERTICAL,i);
			if (direction == Board::UP){
				pushArray(array.begin(),array.end(),stateChanged);
			}
			else {
				pushArray(array.rbegin(),array.rend(),stateChanged);
			}
			if (apply){
				setArray(AXE_VERTICAL,i,array);
			}
		}
	}
	return stateChanged;
}


Board::Board(uint32_t width, uint32_t height):
	impl_(new Impl_)
{
	impl_->init(width,height);
}

Board::~Board(){
}

uint32_t Board::width() const{
	return impl_->width_;
}

uint32_t Board::height() const{
	return impl_->height_;
}

bool Board::push(Board::Direction direction){
	return impl_->testAndPush(direction);
}

uint32_t Board::square(uint32_t x, uint32_t y){
	ENSURE_ACCESS_VALID(impl_,x,y);
	return impl_->values_[x][y];
}

void Board::setSquare(uint32_t x, uint32_t y, uint32_t value){
	ENSURE_ACCESS_VALID(impl_,x,y);
	impl_->values_[x][y] = value;
}

bool Board::isFull() const{
	if (impl_->values_.empty()) return true;
	return emptySquares().empty();
}

bool Board::isMovable() const{
	return
		impl_->testAndPush(LEFT,false) ||
		impl_->testAndPush(RIGHT,false) ||
		impl_->testAndPush(UP,false) ||
		impl_->testAndPush(DOWN,false);
}

std::vector<Board::Pos> Board::emptySquares() const {
	std::vector<Board::Pos> res;
	for (int i = 0;i < width(); i++){
		for (int j = 0; j < height(); j++){
			if (impl_->values_[i][j] == 0){
				res.push_back(Pos(i,j));
			}
		}
	}
	return res;
}

