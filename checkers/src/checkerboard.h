#pragma once

#include <iostream>

#include "checkers_structs.h"
#include "constants.h"



class Checkerboard {
public:
	Checkerboard();
	Checkerboard(std::vector<std::string> board);


	void reset();
	void print();
	char getPiece(BoardPosition boardPosition);
	void executeMove(Move move);
	Checkerboard padBoard(int padding);


private:
	std::vector<std::string> board_;
};
