#include <core/board.h>

#include <vector>
#include <exception>
#include <assert.h>

#include <base.h>

namespace{

enum Axe{
	AXE_VERTICAL = Board::DOWN|Board::UP,
	AXE_HORIZONTAL = Board::LEFT|Board::RIGHT
};

typedef std::vector<Board::ContentValue> Array;

template <class Type>
bool mergeCondition(Type lValue, Type rValue, Type & res){
	if (lValue == rValue){
		res = lValue + rValue;
		return true;
	}
	return false;
}

bool squareIsEmpty(const Board::ContentValue& value){
	return value == 0;
}

void resetSquare(Board::ContentValue& value){
	value = 0;
}

//

void pushArray(Array& array, bool& hasChanged){
	Array::reverse_iterator emptyPos = array.rend();
	Array::reverse_iterator lastValuePos = array.rend();
	Array::reverse_iterator it = array.rbegin();

	for(; it != array.rend(); it++){
		// if current is empty and no empty position has been already defined,
		// save this position
		if (squareIsEmpty(*it)){
			if (emptyPos == array.rend()){
				emptyPos = it;
			}
		}
		else {
			Board::ContentValue res;
			// Current has a value
			// try to merge it with the last value met
			if (lastValuePos != array.rend() && mergeCondition(*it,*lastValuePos,res)){
				// let's do the merge
				*lastValuePos = res;
				resetSquare(*it);
				hasChanged = true;

				// Change the current
				it = lastValuePos;
				// and reset lastValuePos (we don't want merge this new square)
				lastValuePos = array.rend();
			}
			else if (emptyPos != array.rend()){
				// an empty pos is under the current
				// switch current to this empty pos
				*emptyPos = *it;
				resetSquare(*it);
				hasChanged = true;

				// Change the current
				it = emptyPos;
				// this pos become the last value pos
				lastValuePos = it;
				// reset empty pos
				emptyPos = array.rend();
			}
			else{
				lastValuePos = it;
			}

		}
	}
}
}

#define ENSURE_ACCESS_VALID(impl,x,y) \
	if (x >= impl->width_ || y >= impl->height_)\
		throw std::out_of_range("bad access to a square of the board");

class Board::Impl_ : public non_copyable{
public:

	Impl_(uint32_t width, uint32_t height);

	std::vector<std::vector<Board::ContentValue> > values_;
	uint32_t width_;
	uint32_t height_;

	Array getArray(Board::Direction direction, uint32_t index) const;
	void setArray(Board::Direction direction, uint32_t index, Array& data);

	bool testAndPush(Board::Direction direction, bool apply = true);
};

Board::Impl_::Impl_(uint32_t width, uint32_t height):
	width_(width),
	height_(height){

	values_.resize(width);
	for (std::vector<Board::ContentValue>& subValue : values_) {
		subValue.resize(height,0u);
	}
}

Array Board::Impl_::getArray(Board::Direction direction, uint32_t index) const{

	Array data;
	if (direction & AXE_VERTICAL){
		ENSURE_ACCESS_VALID(this,index,0);
		data.resize(height_);
		for (uint32_t i = 0; i < height_; i++){
			uint32_t writeIndex = (direction == Board::DOWN) ? i : height_ - i - 1;
			data[writeIndex] = values_[index][i];
		}
	}
	else{
		ENSURE_ACCESS_VALID(this,0,index);
		data.resize(width_);
		for (uint32_t i = 0; i < width_; i++){
			uint32_t writeIndex = (direction == Board::RIGHT) ? i : width_ - i - 1;
			data[writeIndex] = values_[i][index];
		}
	}
	return data;
}

void Board::Impl_::setArray(Board::Direction direction, uint32_t index, Array& data)
{
	if (direction & AXE_VERTICAL){
		assert(data.size() == height_);
		for (uint32_t i = 0; i < height_; i++){
			uint32_t readIndex = (direction == Board::DOWN) ? i : height_ - i - 1;
			values_[index][i] = data[readIndex];
		}
	}
	else{
		assert(data.size() == width_);
		for (uint32_t i = 0; i < width_; i++){
			uint32_t readIndex = (direction == Board::RIGHT) ? i : width_ - i - 1;
			values_[i][index] = data[readIndex];
		}
	}
}

bool Board::Impl_::testAndPush(Board::Direction direction,bool apply)
{
	bool stateChanged = false;
	if (direction & AXE_HORIZONTAL && width_ > 1){
		for (int i = 0; i < height_; i++){
			Array array = getArray(direction,i);
			pushArray(array,stateChanged);
			if (apply) setArray(direction,i,array);
		}
	}
	else if (direction & AXE_VERTICAL && height_ > 1){
		for (int i = 0;i < width_; i++){
			Array array = getArray(direction,i);
			pushArray(array,stateChanged);
			if (apply) setArray(direction,i,array);
		}
	}
	return stateChanged;
}


Board::Board(uint32_t width, uint32_t height):
	impl_(new Impl_(width,height)){
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

Board::ContentValue Board::square(uint32_t x, uint32_t y){
	ENSURE_ACCESS_VALID(impl_,x,y);
	return impl_->values_[x][y];
}

void Board::setSquare(uint32_t x, uint32_t y, ContentValue value){
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

