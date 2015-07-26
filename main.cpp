#include <core/board.h>

#include <iostream>

int main(int argc, char **argv){

	Board board(2,2);
	std::cout << (int)board.width() <<":" << (int)board.height() << std::endl;

	return 0;
}
