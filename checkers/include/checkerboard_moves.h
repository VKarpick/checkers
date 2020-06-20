/*-------------------------------------------------------------------------------------------------
 Simple class for determining allowable moves on a checkerboard.

 Broken out from other classes to avoid circular references with AI.
-------------------------------------------------------------------------------------------------*/


#pragma once

#include "checkerboard.h"


class CheckerboardMoves {
public:
	// returns the moves a single piece is allowed to make
	static std::vector<Move> piece_moves(Checkerboard checkerboard, CheckersPlayer opponent,
		BoardPosition piece_position, const std::vector<int> row_moves, const bool can_capture);

	// returns the moves allowable moves from the current board
	static std::vector<Move> board_moves(Checkerboard checkerboard, CheckersPlayer current_player, CheckersPlayer opponent);

	static bool is_crowning_move(Checkerboard checkerboard, const char piece, const BoardPosition board_position);
};
