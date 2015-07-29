#include "core/board.h"

#include <vector>
#include <exception>
#include <assert.h>

#include <base.h>

namespace{

enum Axe{
	AXE_VERTICAL = Board::BOTTOM|Board::TOP,
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
void pushArray(ForwardIt itbegin, ForwardIt itEnd){
	ForwardIt empty = itEnd;
	ForwardIt lastValue = itEnd;
	ForwardIt it = itbegin;
	for(; it != itEnd; it++){
		if (*it == 0){
			if (empty == itEnd) empty = it;
		}
		else if (empty != itEnd){
			typename ForwardIt::value_type res;
			if (lastValue != itEnd && mergeCondition(*it,*lastValue,res)){
				*lastValue = res;
				*it = 0;
				it = lastValue;
			}
			else{
				*empty = *it;
				*it = 0;
				it = empty;
				empty = itEnd;
			}
		}
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

	Array getRow(uint32_t row) const;
	void setRow(uint32_t row, Array& data);

	Array getColumn(uint32_t column) const;
	void setColumn(uint32_t column, Array& data);
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

Array Board::Impl_::getRow(uint32_t row) const{
	ENSURE_ACCESS_VALID(this,0,row);
	Array data (width_);
	for (uint32_t i = 0; i < width_; i++)
		data[i] = values_[i][row];
	return data;
}

void Board::Impl_::setRow(uint32_t row, Array& data){
	assert(data.size() == width_);
	for (uint32_t i = 0; i < width_; i++)
		values_[i][row] = data[i];
}

Array Board::Impl_::getColumn(uint32_t column) const{
	ENSURE_ACCESS_VALID(this,column,0);
	Array data (height_);
	for (uint32_t i = 0; i < height_; i++)
		data[i] = values_[column][i];
	return data;
}

void Board::Impl_::setColumn(uint32_t column, Array& data){
	assert(data.size() == height_);
	for (uint32_t i = 0; i < height_; i++)
		values_[column][i] = data[i];
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

void Board::push(Board::Direction direction)
{
	if (direction & AXE_HORIZONTAL && width() > 1)
	{
		for (int i = 0; i < height(); i++){
			if (direction == Board::LEFT){
				Array row = impl_->getRow(i);
				pushArray(row.begin(),row.end());
				impl_->setRow(i,row);
			}
			else {
				Array row = impl_->getRow(i);
				pushArray(row.rbegin(),row.rend());
				impl_->setRow(i,row);
			}
		}
	}
	else if (direction & AXE_VERTICAL && height() > 1)
	{
		for (int i = 0;i < width(); i++){
			if (direction == Board::TOP){
				Array column = impl_->getColumn(i);
				pushArray(column.begin(),column.end());
				impl_->setColumn(i,column);
			}
			else {
				Array column = impl_->getColumn(i);
				pushArray(column.rbegin(),column.rend());
				impl_->setColumn(i,column);
			}
		}
	}
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

std::list<Board::Pos> Board::emptySquares() const {
	std::list<Board::Pos> res;
	for (int i = 0;i < width(); i++){
		for (int j = 0; j < height(); j++){
			if (impl_->values_[i][j] == 0){
				res.push_back(Pos(i,j));
			}
		}
	}
	return res;
}

