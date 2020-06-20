/*-------------------------------------------------------------------------------------------------
 Simple structs useful in Checkers.

 Broken out into separate file to avoid circular references.
-------------------------------------------------------------------------------------------------*/


#pragma once

#include <vector>


struct CheckersPlayer {
public:
	char player;
	int vertical_direction;
	bool is_user_controlled;


	bool has_piece(char piece) {
		return tolower(piece) == player;
	}


	bool operator==(const CheckersPlayer& checkers_player) const {
		return player == checkers_player.player;
	}
};


struct BoardPosition {
	int row;
	int column;


	bool operator==(const BoardPosition& board_position) const {
		return row == board_position.row && column == board_position.column;
	}

	bool operator< (const BoardPosition& board_position) const {
		return row < board_position.row || (row == board_position.row && column < board_position.column);
	}
};



struct Piece {
	BoardPosition position{ -1, 1 };
	char piece;
};



struct Move {
	BoardPosition start_position{ -1, -1 };
	std::vector<BoardPosition> landing_positions;
	bool is_crowning{ false };
	std::vector<Piece> captured_pieces;


	bool operator==(const Move& move) const {
		return start_position == move.start_position && landing_positions == move.landing_positions;
	}
};
