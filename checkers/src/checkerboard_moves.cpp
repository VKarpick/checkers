#include "checkerboard_moves.h"


std::vector<Move> CheckerboardMoves::piece_moves(Checkerboard checkerboard, CheckersPlayer opponent,
	BoardPosition piece_position, const std::vector<int> row_moves, const bool can_capture) {

	std::vector<Move> moves;
	const int move_distance{ (can_capture) ? constants::jump_distance : constants::step_distance };

	for (int row_move : row_moves) {
		for (int column_move : constants::column_moves) {
			const BoardPosition move_position{ piece_position.row + row_move * move_distance, 
				piece_position.column + column_move * move_distance };
			// can only move to open squares
			const bool is_legal_move{ checkerboard.get_piece(move_position) == constants::board_opening };

			bool is_legal_capture{ true };    // initialized to true to simplify if statement
			BoardPosition capture_position;
			if (can_capture) {
				capture_position.row = piece_position.row + row_move * constants::capture_distance;
				capture_position.column = piece_position.column + column_move * constants::capture_distance;
				is_legal_capture = opponent.has_piece(checkerboard.get_piece(capture_position));
			}

			if (is_legal_move && is_legal_capture) {
				Move current_move{
					piece_position,
					{ move_position },
					is_crowning_move(checkerboard, checkerboard.get_piece(piece_position), move_position)
				};

				if (can_capture) {
					current_move.captured_pieces = { Piece{capture_position, checkerboard.get_piece(capture_position)} };

					// for jumps, have to continue jumping until no more jumps available
					Checkerboard board_after_move = checkerboard;
					board_after_move.execute_move(current_move);

					// use recursion to continue jump
					std::vector<Move> moves_continuing_jump{ piece_moves(board_after_move, opponent, move_position, row_moves, can_capture) };

					for (Move jump : moves_continuing_jump) {
						current_move.landing_positions.insert(
							current_move.landing_positions.end(),
							jump.landing_positions.begin(),
							jump.landing_positions.end());

						current_move.captured_pieces.insert(
							current_move.captured_pieces.end(),
							jump.captured_pieces.begin(),
							jump.captured_pieces.end());

						current_move.is_crowning = jump.is_crowning;
					}
				}

				moves.push_back(current_move);
			}
		}
	}

	return moves;
}


std::vector<Move> CheckerboardMoves::board_moves(Checkerboard checkerboard, CheckersPlayer current_player, CheckersPlayer opponent) {
	std::vector<Move> jumps{};
	std::vector<Move> steps{};

	for (BoardPosition position : checkerboard.get_player_positions(current_player)) {
		char piece = checkerboard.get_piece(position);
		std::vector<int> row_moves;

		if (piece == toupper(piece)) {
			row_moves = { -constants::step_distance, constants::step_distance };    // kings can move up or down
		}
		else {
			row_moves = { current_player.vertical_direction };    // regular pieces can only move in their player's direction
		}

		std::vector<Move> piece_jumps{ piece_moves(checkerboard, opponent, position, row_moves, true) };
		jumps.insert(jumps.end(), piece_jumps.begin(), piece_jumps.end());

		// jumps are forced so only need to look for steps if no jumps are available
		if (jumps.empty()) {
			std::vector<Move> piece_steps{ piece_moves(checkerboard, opponent, position, row_moves, false) };
			steps.insert(steps.end(), piece_steps.begin(), piece_steps.end());
		}
	}

	return (jumps.empty()) ? steps : jumps;
}


// pieces are crowned if they aren't already kings and will reach a king's row
bool CheckerboardMoves::is_crowning_move(Checkerboard checkerboard, const char piece, const BoardPosition board_position) {
	const bool is_king{ piece == toupper(piece) };
	return !is_king && checkerboard.is_king_row(board_position);
}
