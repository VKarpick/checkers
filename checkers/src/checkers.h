#pragma once

#include <algorithm>
#include <functional>
#include <map>

#include "checkerboard.h"
#include "checkers_player.h"
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
	std::vector<CheckersPlayer> players_{ CheckersPlayer('r', -1, true), CheckersPlayer('w', 1, true)};
	CheckersPlayer& currentPlayer_{ players_[0] };
	CheckersPlayer& opponent_{ players_[1] };
	std::vector<Move> availableMoveList_{};

	// mapping of allowable non-move input to their corresponding functions
	// eg) "q" -> quit()
	std::map<std::string, std::function<void()>> inputMap;


	// returns the moves a piece is allowed to make
	std::vector<Move> pieceMoves(Checkerboard board, BoardPosition piecePosition, std::vector<int> rowMoves, bool canCapture);

	bool isCrowningMove(char pieceChar, BoardPosition boardPosition);
	void switchPlayers();
	void render();
	std::string getUserInput();
	void processInput(std::string input);
};
