#include <core/board.h>

#include <vector>
#include <list>
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

struct PushArrayReport{
	std::list<std::pair<int,int> > merges_;
	std::list<std::pair<int,int> > moves_;
};

PushArrayReport pushArray(Array& array){
	Array::reverse_iterator lastValuePos = array.rend();
	Array::reverse_iterator it = array.rbegin();

	PushArrayReport report;

	// first pass (merge)
	for(; it != array.rend(); it++){
		if (!squareIsEmpty(*it)){
			Board::ContentValue res;
			// Current has a value
			// try to merge it with the last value met
			if (lastValuePos != array.rend() && mergeCondition(*it,*lastValuePos,res)){
				// let's do the merge
				*lastValuePos = res;
				resetSquare(*it);

				report.merges_.push_back(std::make_pair(
					std::distance(it,array.rend()) -1,
					std::distance(lastValuePos,array.rend())-1));

				// Change the current
				it = lastValuePos;
				// and reset lastValuePos (we don't want merge this new square)
				lastValuePos = array.rend();
			}
			else{
				lastValuePos = it;
			}
		}
	}

	// second pass (move)
	Array::reverse_iterator emptyPos = array.rend();
	for(it = array.rbegin(); it != array.rend(); it++){
		// if current is empty and no empty position has been already defined,
		// save this position
		if (squareIsEmpty(*it)){
			if (emptyPos == array.rend()){
				emptyPos = it;
			}
		}
		else if (emptyPos != array.rend()){
			// an empty pos is under the current
			// switch current to this empty pos
			*emptyPos = *it;
			resetSquare(*it);

			report.moves_.push_back(std::make_pair(
				std::distance(it,array.rend()) -1,
				std::distance(emptyPos,array.rend())-1));

			// Change the current
			it = emptyPos;
			// reset empty pos
			emptyPos = array.rend();
		}
	}
	return report;
}
}

// ****************************************
//         Board Impl
// ****************************************

#define ENSURE_ACCESS_VALID(impl,x,y) \
	if (x >= impl->width_ || y >= impl->height_)\
		throw std::out_of_range("bad access to a square of the board");

class Board::Impl_ : public non_copyable{
public:

	Impl_(uint32_t width, uint32_t height);

	std::vector<std::vector<Board::ContentValue> > values_;
	uint32_t width_;
	uint32_t height_;

	uint32_t getIndex(uint32_t index, Board::Direction direction) const;
	Array getArray(Board::Direction direction, uint32_t index) const;
	void setArray(Board::Direction direction, uint32_t index, Array& data);
	void computeReport(Board::Direction direction, uint32_t index, PushArrayReport arrayReport, Board::Report& report);

	Board::Report testAndPush(Board::Direction direction, bool apply = true);
};

Board::Impl_::Impl_(uint32_t width, uint32_t height):
	width_(width),
	height_(height){

	values_.resize(width);
	for (std::vector<Board::ContentValue>& subValue : values_) {
		subValue.resize(height,0u);
	}
}

uint32_t Board::Impl_::getIndex(uint32_t index, Board::Direction direction) const{
	switch(direction){
		case Board::RIGHT:
		case Board::DOWN: return index;
		case Board::LEFT: return width_ - index - 1;
		case Board::UP: return  height_ - index - 1;
	}
}

Array Board::Impl_::getArray(Board::Direction direction, uint32_t index) const{

	Array data;
	if (direction & AXE_VERTICAL){
		ENSURE_ACCESS_VALID(this,index,0);
		data.resize(height_);
		for (uint32_t i = 0; i < height_; i++){
			data[getIndex(i,direction)] = values_[index][i];
		}
	}
	else{
		ENSURE_ACCESS_VALID(this,0,index);
		data.resize(width_);
		for (uint32_t i = 0; i < width_; i++){
			data[getIndex(i,direction)] = values_[i][index];
		}
	}
	return data;
}

void Board::Impl_::setArray(Board::Direction direction, uint32_t index, Array& data)
{
	if (direction & AXE_VERTICAL){
		assert(data.size() == height_);
		for (uint32_t i = 0; i < height_; i++){
			values_[index][i] = data[getIndex(i,direction)];
		}
	}
	else{
		assert(data.size() == width_);
		for (uint32_t i = 0; i < width_; i++){
			values_[i][index] = data[getIndex(i,direction)];
		}
	}
}

void Board::Impl_::computeReport(Board::Direction direction, uint32_t index, PushArrayReport arrayReport, Board::Report& report)
{
	if (direction & AXE_VERTICAL){
		for(std::pair<int,int> move: arrayReport.moves_){
			report.moves_.push_back(std::make_pair(
				Pos(index,getIndex(move.first,direction)),
				Pos(index,getIndex(move.second,direction))));
		}
		for(std::pair<int,int> merge: arrayReport.merges_){
			report.merges_.push_back(std::make_pair(
				Pos(index,getIndex(merge.first,direction)),
				Pos(index,getIndex(merge.second,direction))));
		}
	}
	else{
		for(std::pair<int,int> move: arrayReport.moves_){
			report.moves_.push_back(std::make_pair(
				Pos(getIndex(move.first,direction),index),
				Pos(getIndex(move.second,direction),index)));
		}
		for(std::pair<int,int> merge: arrayReport.merges_){
			report.merges_.push_back(std::make_pair(
				Pos(getIndex(merge.first,direction),index),
				Pos(getIndex(merge.second,direction),index)));
		}
	}
}

Board::Report Board::Impl_::testAndPush(Board::Direction direction,bool apply)
{
	Board::Report report;
	if (direction & AXE_HORIZONTAL && width_ > 1){
		for (int i = 0; i < height_; i++){
			Array array = getArray(direction,i);
			PushArrayReport arrayReport = pushArray(array);
			computeReport(direction,i,arrayReport,report);
			if (apply) setArray(direction,i,array);
		}
	}
	else if (direction & AXE_VERTICAL && height_ > 1){
		for (int i = 0;i < width_; i++){
			Array array = getArray(direction,i);
			PushArrayReport arrayReport = pushArray(array);
			computeReport(direction,i,arrayReport,report);
			if (apply) setArray(direction,i,array);
		}
	}
	return report;
}


// ****************************************
//         Board class
// ****************************************
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

Board::Report Board::push(Board::Direction direction){
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
		impl_->testAndPush(LEFT,false).changed() ||
		impl_->testAndPush(RIGHT,false).changed() ||
		impl_->testAndPush(UP,false).changed() ||
		impl_->testAndPush(DOWN,false).changed();
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

