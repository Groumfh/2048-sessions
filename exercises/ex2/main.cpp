#include <gtest/gtest.h>

#include <vector>
#include <algorithm>


int sum(const std::vector<int>& values){
	int ret = 0;
	std::for_each(values.begin(), values.end(), [&ret](int n) {ret += n;});
	return ret;
}

// write the sum function
TEST(ex2, test1)
{
	{
		std::vector<int> values({0});
		EXPECT_EQ(sum(values),0);
	}
	{
		std::vector<int> values({4,1,3});
		EXPECT_EQ(sum(values),8);
	}
}

//***********************************************************
bool myComparator(int i, int j) { return (i < j); }
// sort integers
void sortFunction(
	std::vector<int>::iterator begin,
	std::vector<int>::iterator end)
{
	std::sort(begin, end, myComparator);
}

// write the content of sort
TEST(ex2, test2)
{
	std::vector<int> values({4,8,10,6,1});
	std::vector<int> result({1,4,6,8,10});
	sortFunction(values.begin(),values.end());

	EXPECT_EQ(values,result);
}

//***********************************************************

class A
{
public :
	A(bool valid) : valid_(valid) {};
	bool isValid() { return valid_; }
private :
	bool valid_;
};

bool checkElementValidity(A element)
{
	return (!element.isValid());
}

void removeInvalid(std::vector<A>array)
{
	/*std::vector<A>::iterator it = array.begin();
	while (it != array.end())
		it = it->isValid();*/
}

bool checkArrayValidity(std::vector<A>& array)
{
	for (A i : array)
	{
		if (!i.isValid())
			return false;
	}
}

TEST(ex2, test3)
{
	std::vector<A> array = {A(true), A(true),A(false),A(false),A(true),A(false)};

	EXPECT_EQ(6, array.size());
	removeInvalid(array);
	EXPECT_EQ(3, array.size());
	//EXPECT_TRUE(checkArrayValidity(array));
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
