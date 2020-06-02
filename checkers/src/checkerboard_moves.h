#pragma once

#include "checkerboard.h"
#include "checkers_structs.h"



class CheckerboardMoves {
public:
	// returns the moves a piece is allowed to make
	static std::vector<Move> piece_moves(Checkerboard checkerboard, CheckersPlayer opponent,
		BoardPosition piece_position, std::vector<int> row_moves, bool can_capture);

	// returns the moves allowable moves from the current board
	static std::vector<Move> board_moves(Checkerboard checkerboard, CheckersPlayer current_player, CheckersPlayer opponent);

	static bool is_crowning_move(Checkerboard checkerboard, char piece, BoardPosition board_position);
};
