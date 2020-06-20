#include "ai/checkers_environment.h"


std::shared_ptr<State> CheckersEnvironment::reset() {
	n_plays_ = 0;
	state_ = std::make_shared<State>(State{ Checkerboard(constants::starting_board), 0.0, false, players_[0] });
	return state_;
}


std::shared_ptr<State> CheckersEnvironment::step(std::shared_ptr<Action> action) {
	++n_plays_;
	state_ = step(state_, action);

	return state_;
}


std::shared_ptr<State> CheckersEnvironment::step(std::shared_ptr<State> state, std::shared_ptr<Action> action) {
	Checkerboard checkerboard{ checkerboard_from_state(state) };
	checkerboard.execute_move(std::any_cast<Move>(action->action));

	double reward{ 0.0 };

	// different types used for players because a shared_ptr of the opponent is needed for the state being returned
	const CheckersPlayer current_player{ checkers_player_from_state(state) };
	std::shared_ptr<Player> opponent{ opponent_from_state(state) };
	std::vector<Move> opponent_actions{ CheckerboardMoves::board_moves(checkerboard, checkers_player_from_player(opponent), current_player) };

	// game is over if current player has no possible moves
	if (opponent_actions.size() == 0) {
		reward = (current_player.player == constants::pieces[0]) ? 0.001 : -0.001;
	}

	bool is_terminal{ reward != 0.0 || n_plays_ > max_plays_ };

	return std::make_shared<State>(State{ checkerboard, reward, is_terminal, opponent });
}


std::vector<std::shared_ptr<Action>> CheckersEnvironment::get_actions() {
	return get_actions(state_);
}


std::vector<std::shared_ptr<Action>> CheckersEnvironment::get_actions(std::shared_ptr<State> state) {
	std::vector<std::shared_ptr<Action>> actions;
	const Checkerboard checkerboard{ checkerboard_from_state(state) };
	const CheckersPlayer current_player{ checkers_player_from_state(state) };
	const CheckersPlayer opponent{ checkers_player_from_player(opponent_from_state(state)) };
	
	for (Move move : CheckerboardMoves::board_moves(checkerboard, current_player, opponent)) {
		actions.push_back(std::make_shared<Action>(Action{ move }));
	}

	return actions;
}


std::vector<double> CheckersEnvironment::featurize(const std::shared_ptr<State> state) {
	std::vector<double> features;
	std::vector<std::string> board{ checkerboard_from_state(state).get_board() };

	// feature for current player being first or second player
	features.push_back((state->current_player == players_[0]) ? 1.0 : 0.0);

	std::vector<double> n_regular_pieces{ 0.0, 0.0 };
	std::vector<double> n_kings{ 0.0, 0.0 };
	std::vector<double> n_trapped_kings{ 0.0, 0.0 };
	std::vector<double> n_runaways{ 0.0, 0.0 };

	// have to check every accessible square of the board for pieces of each color
	for (size_t row = 0; row < board.size(); ++row) {
		for (size_t column = 0 + (row + 1) % 2; column < board[row].size(); column += 2) {
			for (size_t i = 0; i < constants::pieces.size(); ++i) {
				char piece{ constants::pieces[i] };

				// feature for regular piece in given spot on the board
				double is_regular_piece{ (board[row][column] == piece) ? 1.0 : 0.0 };
				n_regular_pieces[i] += is_regular_piece;
				features.push_back(is_regular_piece);

				// feature for king in given spot on the board
				double is_king{ (board[row][column] == toupper(piece)) ? 1.0 : 0.0 };
				n_kings[i] += is_king;
				features.push_back(is_king);

				const CheckersPlayer current_player{ checkers_player_from_player(players_[(i == 0) ? 0 : 1]) };
				const CheckersPlayer opponent{ checkers_player_from_player(players_[(i == 0) ? 1 : 0]) };
				const BoardPosition piece_position{ int(row), int(column) };

				// check for trapped king
				if (is_king == 1.0) {
					n_trapped_kings[i] += (is_trapped_king(board, piece_position, current_player, opponent)) ? 1.0 : 0.0;
				}

				// check for runaway piece
				if (is_regular_piece == 1.0) {
					n_runaways[i] += (is_runaway(board, piece_position, current_player, opponent) ? 1.0 : 0.0);
				}
			}
		}
	}

	// feature for total number of regular pieces on board
	features.push_back(n_regular_pieces[0]);
	features.push_back(n_regular_pieces[1]);

	// feature for total number of kings on board
	features.push_back(n_kings[0]);
	features.push_back(n_kings[1]);

	// feature for total number of pieces of any kind on board
	features.push_back(n_regular_pieces[0] + n_kings[0]);
	features.push_back(n_regular_pieces[1] + n_kings[1]);

	// feature for kings unable to move
	features.push_back(n_trapped_kings[0]);
	features.push_back(n_trapped_kings[1]);

	// feature for runaway pieces (unimpeded path to be kinged)
	features.push_back(n_runaways[0]);
	features.push_back(n_runaways[1]);

	return features;
}


std::vector<std::shared_ptr<Player>> CheckersEnvironment::get_players() {
	return players_;
}


void CheckersEnvironment::set_state(const std::shared_ptr<State> state) {
	state_ = state;
}


Checkerboard CheckersEnvironment::checkerboard_from_state(const std::shared_ptr<State> state) {
	return std::any_cast<Checkerboard>(state->observation);
}


std::shared_ptr<Player> CheckersEnvironment::opponent_from_state(const std::shared_ptr<State> state) {
	return (state->current_player == players_[0]) ? players_[1] : players_[0];
}


CheckersPlayer CheckersEnvironment::checkers_player_from_state(const std::shared_ptr<State> state) {
	return checkers_player_from_player(state->current_player);
}


CheckersPlayer CheckersEnvironment::checkers_player_from_player(const std::shared_ptr<Player> player) {
	return std::any_cast<CheckersPlayer>(player->player);
}


bool CheckersEnvironment::is_trapped_king(const Checkerboard board, const BoardPosition piece_position, 
	const CheckersPlayer current_player, const CheckersPlayer opponent) {

	// kings can move in any direction
	const std::vector<int> king_row_moves{ -constants::step_distance, constants::step_distance };
	
	// jumps
	std::vector<Move> available_moves{ CheckerboardMoves::piece_moves(board, opponent, piece_position, king_row_moves, true) };

	// steps
	if (available_moves.empty()) {
		available_moves = CheckerboardMoves::piece_moves(board, opponent, piece_position, king_row_moves, false);
	}
	
	// king can't move
	if (available_moves.empty()) {
		return true;
	}
	else {
		// king executing any move results in it being jumped
		for (Move move : available_moves) {
			Checkerboard new_board{ board };
			new_board.execute_move(move);
			BoardPosition king_position{ move.landing_positions.back() };

			if (!can_be_jumped(new_board, king_position, current_player, opponent)) { return false; }
		}
		return true;
	}
}


bool CheckersEnvironment::can_be_jumped(const Checkerboard board, const BoardPosition piece_position,
	const CheckersPlayer current_player, const CheckersPlayer opponent) {

	for (Move opponent_move : CheckerboardMoves::board_moves(board, opponent, current_player)) {
		for (Piece captured_piece : opponent_move.captured_pieces) {
			if (captured_piece.position == piece_position) {
				return true;
			}
		}
	}

	return false;
}


bool CheckersEnvironment::avoid_encounters(Checkerboard board, const BoardPosition piece_position, const CheckersPlayer current_player) {

	const BoardPosition left{ piece_position.row + current_player.vertical_direction, piece_position.column - 1 };
	const BoardPosition right{ piece_position.row + current_player.vertical_direction, piece_position.column + 1 };
	char left_piece{ board.get_piece(left) };
	left_piece = tolower(left_piece);
	char right_piece{ board.get_piece(right) };
	right_piece = tolower(right_piece);

	if ((board.is_king_row(left) && left_piece == constants::board_opening)
		|| (board.is_king_row(right) && right_piece == constants::board_opening)) { 
		
		return true;
	}

	// square to the left is open
	if (left_piece == constants::board_opening) {
		
		// square to the right is open
		// either can lead to an unimpeded path
		if (right_piece == constants::board_opening) {
			return avoid_encounters(board, left, current_player) || avoid_encounters(board, right, current_player);
		}

		// either there is no square to the right or it is occupied by another of the current player's pieces
		// only the path to the left can be unimpeded
		else if (right_piece == ' ' || right_piece == current_player.player) {
			return avoid_encounters(board, left, current_player);
		}

		// opponent is met to the right
		// no unimpeded path exists
		else {
			return false;
		}
	}

	// either there is no square to the left or it is occupied by another of the current player's pieces
	else if (left_piece == ' ' || left_piece == current_player.player) {

		// square to the right is open
		// only the path to the right can be unimpeded
		if (right_piece == constants::board_opening) {
			return avoid_encounters(board, right, current_player);
		}

		// opponent is met to the right
		// no unimpeded path exists
		else {
			return false;
		}
	}

	// opponent is met to the left
	// no unimpeded path exists
	else {
		return false;
	}
}


bool CheckersEnvironment::is_runaway(const Checkerboard board, const BoardPosition piece_position,
	const CheckersPlayer current_player, const CheckersPlayer opponent) {

	// if piece can currently be jumped, can't be a runaway
	if (can_be_jumped(board, piece_position, current_player, opponent)) {
		return false;
	}
	else {
		return avoid_encounters(board, piece_position, current_player);
	}
}
