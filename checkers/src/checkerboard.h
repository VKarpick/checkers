#pragma once

#include <iostream>

#include "checkers_player.h"
#include "checkers_structs.h"
#include "constants.h"



class Checkerboard {
public:
	Checkerboard();
	Checkerboard(std::vector<std::string> board);


	std::vector<std::string> getBoard();


	void reset();
	char getPiece(BoardPosition boardPosition);
	std::vector<BoardPosition> getPlayerPositions(CheckersPlayer player);
	void executeMove(Move move);
	void reverseMove(Move move);


private:
	std::vector<std::string> board_;
};



std::ostream& operator<< (std::ostream& out, Checkerboard& checkerboard);
