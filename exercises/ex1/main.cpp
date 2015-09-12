#include <gtest/gtest.h>
#include <math.h>

//***********************************************************

// return value if it's equal to 5 otherwise return 0
int error1Function(int value){
	if ((value = 5)){
		return value;
	}
	return 0;
}

// fix error1Function to match this test case
TEST(ex1, error1)
{
	EXPECT_EQ(error1Function(5), 5);
	EXPECT_EQ(error1Function(1), 0);
	EXPECT_EQ(error1Function(4), 0);
}


//***********************************************************

// calculate square root of value
// return if calcul have succeed
bool error2Function(int value){
	if (value == 0) return false;
	value = sqrt(value);
	return true;
}

// fix error2Function to match this test case
TEST(ex1, error2)
{
	{
		int value = 0;
		EXPECT_FALSE(error2Function(value));
		EXPECT_EQ(value, 0);
	}
	{
		int value = 9;
		EXPECT_TRUE(error2Function(value));
		EXPECT_EQ(value, 3);
	}
}


//***********************************************************

class Dog{
public:
	Dog (bool player):player_(player){}
	bool wantToPlay() const {return player_;}
private:
	bool player_;
};

class Human{
public:
	bool wantToWork() const {return true;}
};

// test if good conditions is here for writing code
bool error3Function(Dog* dog, Human * human)
{
	return false;
}

// write error3Function content to match this test case
TEST(ex1, error3)
{
	EXPECT_FALSE(error3Function(NULL,NULL));

	Human me;
	EXPECT_TRUE(error3Function(NULL,&me));

	Dog myfirstDog(true);
	EXPECT_FALSE(error3Function(&myfirstDog,NULL));
	EXPECT_FALSE(error3Function(&myfirstDog,&me));

	Dog mysecondDog(false);
	EXPECT_FALSE(error3Function(&mysecondDog,NULL));
	EXPECT_TRUE(error3Function(&mysecondDog,&me));
}

//***********************************************************

// calculate the sum of array content
// advanced (write a recursive function)
int error4Function(int* array, int size)
{
	return 0;
}

// write error4Function content to match this test case
TEST(ex1, error4)
{
	{
		int array []= { 0 };
		EXPECT_EQ(error4Function(array,1), 0);
	}

	{
		int array []= { 4,1,3 };
		EXPECT_EQ(error4Function(array,3), 8);
	}
}


//***********************************************************


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
