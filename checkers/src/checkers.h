#pragma once

#include <algorithm>
#include <functional>
#include <map>

#include "checkerboard.h"
#include "checkers_structs.h"
#include "constants.h"



class Checkers {
public:
	Checkers();


	void play();
	void reset();
	void update();    // updates availableMoveList
	void quit();


	//TODO remove these
	void printMoveBoards();
	void boardTry();
	void executeInputMove(int moveIndex);


private:
	Checkerboard checkerboard_;
	char currentPlayerChar_{ constants::kPlayerChars[0] };
	char opponentChar_{ constants::kPlayerChars[1] };
	std::vector<Move> availableMoveList_{};
	std::map<std::string, std::function<void()>> inputMap;


	// returns vertical direction a piece is allowed to move
	std::vector<int> allowableRowMoves(char pieceChar);

	// returns the moves a piece is allowed to make
	std::vector<Move> pieceMoves(Checkerboard board, BoardPosition piecePosition, 
		int moveDistance, std::vector<int> rowMoves, bool canCapture);

	bool isCrowningMove(char pieceChar, BoardPosition boardPosition);
	void switchPlayers();
	void render();
	std::string getUserInput();
	void processInput(std::string input);
};
