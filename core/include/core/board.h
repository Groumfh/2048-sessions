#pragma once

#include <memory>
#include <list>

class Board
{
public:

	enum Direction{
		TOP     = 1,
		BOTTOM  = 2,
		LEFT    = 4,
		RIGHT   = 8
	};

	struct Pos{
		Pos(uint32_t xCoord, uint32_t yCoord):x(xCoord),y(yCoord){}
		uint32_t x;
		uint32_t y;

		bool operator == (const Pos& pos) const{
			return pos.x == x && pos.y == y;
		}
	};

	Board(uint32_t width, uint32_t height);
	~Board();

	uint32_t square(uint32_t x, uint32_t y);
	void setSquare(uint32_t x, uint32_t y, uint32_t value);

	uint32_t height() const;
	uint32_t width() const;

	bool isFull() const;

	std::list<Pos> emptySquares() const;

	void push(Direction direction);

private:
	class Impl_;
	std::unique_ptr<Impl_> impl_;
};
