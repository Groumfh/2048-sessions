#include <gtest/gtest.h>
#include <core/board.h>

TEST(board, init)
{
	{
		Board board(0,0);

		EXPECT_EQ(board.width(),0);
		EXPECT_EQ(board.height(),0);
		EXPECT_ANY_THROW(board.square(1,1));
	}

	{
		Board board(1,2);

		EXPECT_EQ(board.width(),1);
		EXPECT_EQ(board.height(),2);
		EXPECT_NO_THROW(board.square(0,1));
		EXPECT_EQ(board.square(0,1),0);
	}

	{
		Board board(5,5);
		EXPECT_EQ(board.width(),5);
		EXPECT_EQ(board.height(),5);
		for (int i = 0; i < board.width(); i++){
			for (int j = 0; j < board.height(); j++){
				EXPECT_EQ(board.square(i,j),0);
			}
		}
	}
}

TEST(board, setValue)
{
	Board board(3,3);
	uint32_t res = 0;
	EXPECT_NO_THROW(res = board.square(1,1));
	EXPECT_EQ(res,0);
	const uint32_t valueSet = 5;
	EXPECT_ANY_THROW(board.setSquare(20,20,valueSet));
	EXPECT_NO_THROW(board.setSquare(1,1,valueSet));
	EXPECT_EQ(board.square(1,1),valueSet);
}


TEST(board, isFull)
{
	EXPECT_TRUE(Board(0,0).isFull());
	EXPECT_TRUE(Board(0,0).emptySquares().empty());
	EXPECT_FALSE(Board(1,1).isFull());
	EXPECT_FALSE(Board(1,1).emptySquares().empty());
	EXPECT_EQ(Board(1,1).emptySquares().front(),Board::Pos(0,0));
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
		EXPECT_EQ(board.emptySquares().front(),Board::Pos(0,0));
		board.setSquare(0,0,10);
		EXPECT_TRUE(board.isFull());
		EXPECT_TRUE(board.emptySquares().empty());
	}
}

TEST(board, pushOneValue)
{
	Board board(3,3);
	const uint32_t value = 5;

	board.setSquare(0,0,value);

	EXPECT_EQ(board.square(0,0),value);
	EXPECT_EQ(board.square(0,2),0);
	EXPECT_EQ(board.square(2,2),0);
	EXPECT_EQ(board.square(2,0),0);

	board.push(Board::RIGHT);

	EXPECT_EQ(board.square(0,0),0);
	EXPECT_EQ(board.square(2,0),value);
	EXPECT_EQ(board.square(2,2),0);
	EXPECT_EQ(board.square(0,2),0);

	board.push(Board::BOTTOM);

	EXPECT_EQ(board.square(0,0),0);
	EXPECT_EQ(board.square(2,0),0);
	EXPECT_EQ(board.square(2,2),value);
	EXPECT_EQ(board.square(0,2),0);

	board.push(Board::LEFT);

	EXPECT_EQ(board.square(0,0),0);
	EXPECT_EQ(board.square(2,0),0);
	EXPECT_EQ(board.square(2,2),0);
	EXPECT_EQ(board.square(0,2),value);

	board.push(Board::TOP);

	EXPECT_EQ(board.square(0,0),value);
	EXPECT_EQ(board.square(0,2),0);
	EXPECT_EQ(board.square(2,2),0);
	EXPECT_EQ(board.square(2,0),0);
}

TEST(board, pushTwoValues)
{
	Board board(5,1);
	const uint32_t value = 5;
	const uint32_t value2 = 7;

	board.setSquare(0,0,value);
	board.setSquare(2,0,value2);

	EXPECT_EQ(board.square(0,0),value);
	EXPECT_EQ(board.square(1,0),0);
	EXPECT_EQ(board.square(2,0),value2);
	EXPECT_EQ(board.square(3,0),0);
	EXPECT_EQ(board.square(4,0),0);

	board.push(Board::RIGHT);

	EXPECT_EQ(board.square(0,0),0);
	EXPECT_EQ(board.square(1,0),0);
	EXPECT_EQ(board.square(2,0),0);
	EXPECT_EQ(board.square(3,0),value);
	EXPECT_EQ(board.square(4,0),value2);
}

TEST(board, mergeTwoValuesEqual)
{
	{
		Board board(5,1);
		const uint32_t value = 5;
		const uint32_t total = 10;

		board.setSquare(0,0,value);
		board.setSquare(2,0,value);

		EXPECT_EQ(board.square(0,0),value);
		EXPECT_EQ(board.square(1,0),0);
		EXPECT_EQ(board.square(2,0),value);
		EXPECT_EQ(board.square(3,0),0);
		EXPECT_EQ(board.square(4,0),0);

		board.push(Board::RIGHT);

		EXPECT_EQ(board.square(0,0),0);
		EXPECT_EQ(board.square(1,0),0);
		EXPECT_EQ(board.square(2,0),0);
		EXPECT_EQ(board.square(3,0),0);
		EXPECT_EQ(board.square(4,0),total);
	}

	{
		Board board(5,1);
		const uint32_t value = 5;
		const uint32_t total = 10;

		board.setSquare(0,0,value);
		board.setSquare(1,0,value);
		board.setSquare(2,0,value);
		board.setSquare(4,0,value);

		EXPECT_EQ(board.square(0,0),value);
		EXPECT_EQ(board.square(1,0),value);
		EXPECT_EQ(board.square(2,0),value);
		EXPECT_EQ(board.square(3,0),0);
		EXPECT_EQ(board.square(4,0),value);

		board.push(Board::RIGHT);

		EXPECT_EQ(board.square(0,0),0);
		EXPECT_EQ(board.square(1,0),0);
		EXPECT_EQ(board.square(2,0),0);
		EXPECT_EQ(board.square(3,0),total);
		EXPECT_EQ(board.square(4,0),total);
	}
}


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
