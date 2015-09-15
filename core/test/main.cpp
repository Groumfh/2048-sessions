#include <gtest/gtest.h>
#include <core/board.h>

#include <iostream>

// used by gtest for error display
void PrintTo(const Board::Pos& pos, ::std::ostream* os) {
	*os << "("<< pos.x <<","<< pos.y << ")";
}

TEST(board, init)
{
	{
		Board board(0,0);

		EXPECT_EQ(0,board.width());
		EXPECT_EQ(0,board.height());
		EXPECT_ANY_THROW(board.square(1,1));
	}

	{
		Board board(1,2);

		EXPECT_EQ(1,board.width());
		EXPECT_EQ(2,board.height());
		EXPECT_NO_THROW(board.square(0,1));
		EXPECT_EQ(0,board.square(0,1));
	}

	{
		Board board(5,5);
		EXPECT_EQ(5,board.width());
		EXPECT_EQ(5,board.height());
		for (int i = 0; i < board.width(); i++){
			for (int j = 0; j < board.height(); j++){
				EXPECT_EQ(0,board.square(i,j));
			}
		}
	}
}

TEST(board, setValue)
{
	Board board(3,3);
	uint32_t res = 0;
	EXPECT_NO_THROW(res = board.square(1,1));
	EXPECT_EQ(0,res);
	const uint32_t valueSet = 5;
	EXPECT_ANY_THROW(board.setSquare(20,20,valueSet));
	EXPECT_NO_THROW(board.setSquare(1,1,valueSet));
	EXPECT_EQ(valueSet,board.square(1,1));
}


TEST(board, isFull)
{
	EXPECT_TRUE(Board(0,0).isFull());
	EXPECT_TRUE(Board(0,0).emptySquares().empty());
	EXPECT_FALSE(Board(1,1).isFull());
	EXPECT_FALSE(Board(1,1).emptySquares().empty());
	EXPECT_EQ(Board::Pos(0,0),Board(1,1).emptySquares().front());
	{
		Board board(1,1);
		board.setSquare(0,0,10);
		EXPECT_TRUE(board.isFull());
		EXPECT_TRUE(Board(0,0).emptySquares().empty());
	}

	{
		Board board(3,3);
		int value = 0;
		for (int i = 0; i < board.width(); i++){
			for (int j = 0; j < board.height(); j++){
				board.setSquare(i,j,value ++);
			}
		}
		EXPECT_FALSE(board.isFull());
		EXPECT_EQ(Board::Pos(0,0),board.emptySquares().front());
		board.setSquare(0,0,10);
		EXPECT_TRUE(board.isFull());
		EXPECT_TRUE(board.emptySquares().empty());
	}
}

TEST(board, pushOneValue)
{
	Board board(3,3);
	const uint32_t value = 5;
	Board::Report report;

	board.setSquare(0,0,value);

	EXPECT_EQ(value,board.square(0,0));
	EXPECT_EQ(0,	board.square(0,2));
	EXPECT_EQ(0,	board.square(2,2));
	EXPECT_EQ(0,	board.square(2,0));

	report = board.push(Board::RIGHT);
	ASSERT_TRUE(report.moves_.size() == 1);
	EXPECT_EQ(Board::Pos(0,0), report.moves_.front().first);
	EXPECT_EQ(Board::Pos(2,0), report.moves_.front().second);
	EXPECT_TRUE(report.changed());

	EXPECT_EQ(0,	board.square(0,0));
	EXPECT_EQ(value,board.square(2,0));
	EXPECT_EQ(0,	board.square(2,2));
	EXPECT_EQ(0,	board.square(0,2));

	report = board.push(Board::DOWN);
	ASSERT_TRUE(report.moves_.size() == 1);
	EXPECT_EQ(Board::Pos(2,0), report.moves_.front().first);
	EXPECT_EQ(Board::Pos(2,2), report.moves_.front().second);
	EXPECT_TRUE(report.changed());

	EXPECT_EQ(0,	board.square(0,0));
	EXPECT_EQ(0,	board.square(2,0));
	EXPECT_EQ(value,board.square(2,2));
	EXPECT_EQ(0,	board.square(0,2));

	report = board.push(Board::LEFT);
	ASSERT_TRUE(report.moves_.size() == 1);
	EXPECT_EQ(Board::Pos(2,2), report.moves_.front().first);
	EXPECT_EQ(Board::Pos(0,2), report.moves_.front().second);
	EXPECT_TRUE(report.changed());

	EXPECT_EQ(0,	board.square(0,0));
	EXPECT_EQ(0,	board.square(2,0));
	EXPECT_EQ(0,	board.square(2,2));
	EXPECT_EQ(value,board.square(0,2));

	report = board.push(Board::UP);
	ASSERT_TRUE(report.moves_.size() == 1);
	EXPECT_EQ(Board::Pos(0,2), report.moves_.front().first);
	EXPECT_EQ(Board::Pos(0,0), report.moves_.front().second);
	EXPECT_TRUE(report.changed());

	EXPECT_EQ(value,board.square(0,0));
	EXPECT_EQ(0,	board.square(0,2));
	EXPECT_EQ(0,	board.square(2,2));
	EXPECT_EQ(0,	board.square(2,0));
}

TEST(board, pushReport)
{
	Board board(5,1);
	const uint32_t value = 5;

	board.setSquare(2,0,value);

	Board::Report report = board.push(Board::RIGHT);
	EXPECT_EQ(value,board.square(4,0));
	ASSERT_EQ(1,report.moves_.size());
	EXPECT_EQ(Board::Pos(2,0), report.moves_[0].first);
	EXPECT_EQ(Board::Pos(4,0), report.moves_[0].second);
	EXPECT_TRUE(report.changed());
}

TEST(board, pushtest)
{
	Board board(3,1);
	const uint32_t value = 5;

	board.setSquare(2,0,value);

	EXPECT_EQ(0,	board.square(0,0));
	EXPECT_EQ(0,	board.square(1,0));
	EXPECT_EQ(value,board.square(2,0));

	EXPECT_FALSE(board.push(Board::RIGHT).changed());

	EXPECT_EQ(0,	board.square(0,0));
	EXPECT_EQ(0,	board.square(1,0));
	EXPECT_EQ(value,board.square(2,0));
}

TEST(board, pushTwoValues)
{
	Board board(5,1);
	const uint32_t value = 5;
	const uint32_t value2 = 7;

	board.setSquare(0,0,value);
	board.setSquare(2,0,value2);

	EXPECT_EQ(value,board.square(0,0));
	EXPECT_EQ(0,	board.square(1,0));
	EXPECT_EQ(value2,board.square(2,0));
	EXPECT_EQ(0,	board.square(3,0));
	EXPECT_EQ(0,	board.square(4,0));

	Board::Report report = board.push(Board::RIGHT);
	ASSERT_EQ(2,report.moves_.size());
	EXPECT_EQ(Board::Pos(2,0), report.moves_[0].first);
	EXPECT_EQ(Board::Pos(4,0), report.moves_[0].second);
	EXPECT_EQ(Board::Pos(0,0), report.moves_[1].first);
	EXPECT_EQ(Board::Pos(3,0), report.moves_[1].second);
	EXPECT_TRUE(report.changed());

	EXPECT_EQ(0,	board.square(0,0));
	EXPECT_EQ(0,	board.square(1,0));
	EXPECT_EQ(0,	board.square(2,0));
	EXPECT_EQ(value,board.square(3,0));
	EXPECT_EQ(value2,board.square(4,0));

	EXPECT_FALSE(board.push(Board::RIGHT).changed());
}

TEST(board, mergeTwoValuesEqualSticked)
{
	Board board(3,1);
	const uint32_t value = 5;
	const uint32_t total = 10;

	board.setSquare(1,0,value);
	board.setSquare(2,0,value);

	EXPECT_EQ(0,		board.square(0,0));
	EXPECT_EQ(value,	board.square(1,0));
	EXPECT_EQ(value,	board.square(2,0));

	Board::Report report = board.push(Board::RIGHT);
	ASSERT_EQ(1,report.merges_.size());
	EXPECT_EQ(Board::Pos(1,0), report.merges_[0].first);
	EXPECT_EQ(Board::Pos(2,0), report.merges_[0].second);
	ASSERT_EQ(0,report.moves_.size());
	EXPECT_TRUE(report.changed());

	EXPECT_EQ(0,		board.square(0,0));
	EXPECT_EQ(0,		board.square(1,0));
	EXPECT_EQ(total,	board.square(2,0));
}

TEST(board, mergeTwoValuesEqualSeparated)
{
	Board board(4,1);
	const uint32_t value = 5;
	const uint32_t total = 10;

	board.setSquare(0,0,value);
	board.setSquare(2,0,value);

	EXPECT_EQ(value,board.square(0,0));
	EXPECT_EQ(0,	board.square(1,0));
	EXPECT_EQ(value,board.square(2,0));
	EXPECT_EQ(0,	board.square(3,0));

	Board::Report report = board.push(Board::RIGHT);
	ASSERT_EQ(1,report.merges_.size());
	EXPECT_EQ(Board::Pos(0,0), report.merges_[0].first);
	EXPECT_EQ(Board::Pos(2,0), report.merges_[0].second);
	ASSERT_EQ(1,report.moves_.size());
	EXPECT_EQ(Board::Pos(2,0), report.moves_[0].first);
	EXPECT_EQ(Board::Pos(3,0), report.moves_[0].second);
	EXPECT_TRUE(report.changed());

	EXPECT_EQ(0,	board.square(0,0));
	EXPECT_EQ(0,	board.square(1,0));
	EXPECT_EQ(0,	board.square(2,0));
	EXPECT_EQ(total,board.square(3,0));
}

TEST(board, twoMergeInOnePush)
{
	Board board(5,1);
	const uint32_t value = 5;
	const uint32_t total = 10;

	board.setSquare(0,0,value);
	board.setSquare(1,0,value);
	board.setSquare(2,0,value);
	board.setSquare(4,0,value);

	EXPECT_EQ(value,board.square(0,0));
	EXPECT_EQ(value,board.square(1,0));
	EXPECT_EQ(value,board.square(2,0));
	EXPECT_EQ(0,	board.square(3,0));
	EXPECT_EQ(value,board.square(4,0));

	Board::Report report = board.push(Board::RIGHT);
	ASSERT_EQ(2,report.merges_.size());
	EXPECT_EQ(Board::Pos(2,0), report.merges_[0].first);
	EXPECT_EQ(Board::Pos(4,0), report.merges_[0].second);
	EXPECT_EQ(Board::Pos(0,0), report.merges_[1].first);
	EXPECT_EQ(Board::Pos(1,0), report.merges_[1].second);
	ASSERT_EQ(1,report.moves_.size());
	EXPECT_EQ(Board::Pos(1,0), report.moves_[0].first);
	EXPECT_EQ(Board::Pos(3,0), report.moves_[0].second);
	EXPECT_TRUE(report.changed());

	EXPECT_EQ(0,	board.square(0,0));
	EXPECT_EQ(0,	board.square(1,0));
	EXPECT_EQ(0,	board.square(2,0));
	EXPECT_EQ(total,board.square(3,0));
	EXPECT_EQ(total,board.square(4,0));
}

TEST(board, mergeIsNotChained)
{
	Board board(4,1);

	board.setSquare(0,0,8);
	board.setSquare(1,0,4);
	board.setSquare(2,0,2);
	board.setSquare(3,0,2);

	EXPECT_EQ(8,	board.square(0,0));
	EXPECT_EQ(4,	board.square(1,0));
	EXPECT_EQ(2,	board.square(2,0));
	EXPECT_EQ(2,	board.square(3,0));

	EXPECT_TRUE(board.push(Board::RIGHT).changed());

	EXPECT_EQ(0,	board.square(0,0));
	EXPECT_EQ(8,	board.square(1,0));
	EXPECT_EQ(4,	board.square(2,0));
	EXPECT_EQ(4,	board.square(3,0));
}

TEST(board, movable)
{
	{ // empty board
		Board board(2,2);
		EXPECT_FALSE(board.isMovable());
	}
	{ // full board
		Board board(2,2);
		board.setSquare(0,0, 32);
		board.setSquare(1,0, 43);
		board.setSquare(1,1, 58);
		board.setSquare(0,1, 97);
		EXPECT_FALSE(board.isMovable());
		board.setSquare(0,0, 0);
		EXPECT_TRUE(board.isMovable());
	}
}

TEST(achievement, succes)
{

}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
