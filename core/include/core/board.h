#pragma once

#include <memory>

class Board
{
public:
	Board(uint32_t width, uint32_t height);
	~Board();

	uint32_t square(uint32_t x, uint32_t y);
	void setSquare(uint32_t x, uint32_t y, uint32_t value);

	uint32_t height();
	uint32_t width();

	enum Direction
	{
		TOP     = 1,
		BOTTOM  = 2,
		LEFT    = 4,
		RIGHT   = 8
	};

	void push(Direction direction);

private:
	class Impl_;
	std::unique_ptr<Impl_> impl_;
};
