#pragma once

#include <memory>
#include <vector>
#include <list>

class Score
{
public:
	Score();

	std::string getValue();
	void setValue(int INPUT) { m_Value = INPUT; }
	void add(int INPUT) { setValue(m_Value + INPUT); }

private:
	int m_Value;
};

class Board
{
public:

	enum Direction{
		UP      = 1,
		DOWN    = 2,
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

	struct Report{
		std::vector<std::pair<Pos,Pos>> moves_;
		std::vector<std::pair<Pos,Pos>> merges_;
		bool changed() const{ return !moves_.empty() || !merges_.empty();}
	};

	typedef uint32_t ContentValue;

	Board(uint32_t width, uint32_t height);
	~Board();

	ContentValue square(uint32_t x, uint32_t y);
	void setSquare(uint32_t x, uint32_t y, ContentValue value);
	int getSquare(uint32_t x, uint32_t y);

	uint32_t height() const;
	uint32_t width() const;

	bool isFull() const;
	bool isMovable() const;

	std::vector<Pos> emptySquares() const;

	Report push(Direction direction);

private:
	class Impl_;
	std::unique_ptr<Impl_> impl_;
};