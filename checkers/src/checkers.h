#pragma once

#include "checkerboard.h"
#include "checkers_structs.h"
#include "constants.h"



class Checkers {
public:
	void play();
	void reset();
	void update();    // updates availableMoveList


	//TODO remove these
	void printMoveBoards();
	void boardTry();


private:
	Checkerboard checkerboard_;
	char currentPlayerChar_{ constants::kPlayerChars[0] };
	char opponentChar_{ constants::kPlayerChars[1] };
	std::vector<Move> availableMoveList_{};


	// returns vertical direction a piece is allowed to move
	std::vector<int> allowableRowMoves(char pieceChar);

	// returns the moves a piece is allowed to make
	std::vector<Move> pieceMoves(Checkerboard board, BoardPosition piecePosition, 
		int moveDistance, std::vector<int> rowMoves, bool canCapture);

	bool isCrowning(char pieceChar, BoardPosition boardPosition);
};
