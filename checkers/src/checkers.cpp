#include "checkers.h"



bool is_int(const std::string& s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}



Checkers::Checkers() {
	input_map["q"] = std::bind(&Checkers::quit, this);
	input_map["quit"] = std::bind(&Checkers::quit, this);
	input_map["exit"] = std::bind(&Checkers::quit, this);
	
	input_map["r"] = std::bind(&Checkers::random_move, this);
	input_map["rand"] = std::bind(&Checkers::random_move, this);
	input_map["random"] = std::bind(&Checkers::random_move, this);
	
	input_map["u"] = std::bind(&Checkers::undo, this); 
	input_map["z"] = std::bind(&Checkers::undo, this);
	input_map["undo"] = std::bind(&Checkers::undo, this);

	input_map["y"] = std::bind(&Checkers::redo, this);
	input_map["redo"] = std::bind(&Checkers::redo, this);

	//TODO change this to new game screen when have states
	input_map["n"] = std::bind(&Checkers::play, this);
	input_map["new"] = std::bind(&Checkers::play, this);
	input_map["newgame"] = std::bind(&Checkers::play, this);
	input_map["new game"] = std::bind(&Checkers::play, this);
}


//std::vector<Move> Checkers::piece_moves(Checkerboard checkerboard, CheckersPlayer opponent,
//	BoardPosition piece_position, std::vector<int> row_moves, bool can_capture) {
//
//	std::vector<Move> moves;
//	int move_distance{ (can_capture) ? constants::jump_distance : constants::step_distance };
//
//	for (int row_move : row_moves) {
//		for (int column_move : constants::column_moves) {
//			BoardPosition move_position{ piece_position.row + row_move * move_distance, piece_position.column + column_move * move_distance };
//			// can only move to open squares
//			bool is_legal_move{ checkerboard.get_piece(move_position) == constants::board_opening };
//
//			bool is_legal_capture{ true };    // initialized to true to simplify if statement
//			BoardPosition capture_position;
//			if (can_capture) {
//				capture_position.row = piece_position.row + row_move * constants::capture_distance;
//				capture_position.column = piece_position.column + column_move * constants::capture_distance;
//				is_legal_capture = opponent.has_piece(checkerboard.get_piece(capture_position));
//			}
//
//			if (is_legal_move && is_legal_capture) {
//				Move current_move{ 
//					piece_position, 
//					{ move_position }, 
//					is_crowning_move(checkerboard, checkerboard.get_piece(piece_position), move_position)
//				};
//
//				if (can_capture) {
//					current_move.captured_pieces = { Piece{capture_position, checkerboard.get_piece(capture_position)} };
//
//					// for jumps, have to continue jumping until no more jumps available
//					Checkerboard board_after_move = checkerboard;
//					board_after_move.execute_move(current_move);
//
//					// use recursion to continue jump
//					std::vector<Move> moves_continuing_jump{ piece_moves(board_after_move, opponent, move_position, row_moves, can_capture) };
//
//					for (Move jump : moves_continuing_jump) {
//						current_move.landing_positions.insert(
//							current_move.landing_positions.end(),
//							jump.landing_positions.begin(),
//							jump.landing_positions.end());
//
//						current_move.captured_pieces.insert(
//							current_move.captured_pieces.end(),
//							jump.captured_pieces.begin(),
//							jump.captured_pieces.end());
//
//						current_move.is_crowning = jump.is_crowning;
//					}
//				}
//
//				moves.push_back(current_move);
//			}
//		}
//	}
//
//	return moves;
//}
//
//
//std::vector<Move> Checkers::board_moves(Checkerboard checkerboard, CheckersPlayer current_player, CheckersPlayer opponent) {
//	std::vector<Move> jumps{};
//	std::vector<Move> steps{};
//
//	for (BoardPosition position : checkerboard.get_player_positions(current_player)) {
//		char piece = checkerboard.get_piece(position);
//		std::vector<int> row_moves;
//
//		if (piece == toupper(piece)) {
//			row_moves = { -1, 1 };    // kings can move up or down
//		}
//		else {
//			row_moves = { current_player.vertical_direction };    // regular pieces can only move in their player's direction
//		}
//
//		std::vector<Move> piece_jumps{ piece_moves(checkerboard, opponent, position, row_moves, true) };
//		jumps.insert(jumps.end(), piece_jumps.begin(), piece_jumps.end());
//
//		// jumps are forced so only need to look for steps if no jumps are available
//		if (jumps.empty()) {
//			std::vector<Move> piece_steps{ piece_moves(checkerboard, opponent, position, row_moves, false) };
//			steps.insert(steps.end(), piece_steps.begin(), piece_steps.end());
//		}
//	}
//
//	return (jumps.empty()) ? steps : jumps;
//}
//
//
//// pieces are crowned if they aren't already kings and can reach the top/bottom of the board
//bool Checkers::is_crowning_move(Checkerboard checkerboard, char piece, BoardPosition board_position) {
//	bool is_king{ piece == toupper(piece) };
//	return !is_king && checkerboard.is_king_row(board_position);
//}


std::vector<Move> Checkers::get_available_moves() {
	return available_moves_;
}


void Checkers::play() {
	reset();

	while (!available_moves_.empty()) {
		render();

		std::string input;
		if (current_player_.is_user_controlled) {
			input = get_user_input();
		}
		else {
			input = ai_input();
		}

		process_input(input);
		
		update();
	}
}


void Checkers::reset() {
	checkerboard_.reset();
	current_player_ = players_[0];
	opponent_ = players_[1];
	previous_moves_.clear();
	redo_moves_.clear();
	update();
}


void Checkers::update() {
	available_moves_ = CheckerboardMoves::board_moves(checkerboard_, current_player_, opponent_);
}


void Checkers::quit() {
	exit(0);
}


void Checkers::switch_players() {
	std::swap(current_player_, opponent_);
}


void Checkers::render() {
	//TODO split this into renderBoard and renderOptions
	std::cout << std::endl;
	std::cout << checkerboard_ << std::endl;

	int i{ 1 };
	for (Move move : available_moves_) {
		std::cout << i++ << ") ";
		std::cout << "(" << move.start_position.row << ", " << move.start_position.column << ")";
		for (BoardPosition landing_position : move.landing_positions) {
			std::cout << " to (" << landing_position.row << ", " << landing_position.column << ")";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


std::string Checkers::get_user_input() {
	std::cin.clear();
	std::string user_input;
	std::getline(std::cin, user_input);
	
	// convert to lower case
	std::transform(user_input.begin(), user_input.end(), user_input.begin(), [](unsigned char c) { return tolower(c); });
	
	return user_input;
}


void Checkers::process_input(std::string input) {
	if (input_map.count(input) == 1) {
		// pre-defined allowable input
		input_map[input]();
	}
	else {
		if (is_int(input)) {
			int move_index{ std::stoi(input) - 1};
			if (-1 < move_index && move_index < available_moves_.size()) {
				
				// number representing which available move to make
				make_move(available_moves_[move_index], true);
			}
		}
	}
}


void Checkers::random_move() {
	make_move(available_moves_[rand() % available_moves_.size()], true);
}


void Checkers::make_move(Move move, bool is_new_move) {
	checkerboard_.execute_move(move);
	previous_moves_.push_back(move);    // update for potential undo

	// if not a redo, need to clear the redo list
	if (is_new_move) {
		redo_moves_.clear();
	}

	switch_players();
}


void Checkers::undo() {
	// if single player game, need to reverse 2 moves to get back to user's last play
	int moves_to_reverse{ (opponent_.is_user_controlled) ? 1 : 2 };

	if (moves_to_reverse <= previous_moves_.size()) {
		for (int i = 0; i < moves_to_reverse; ++i) {
			Move move{ previous_moves_.back() };
			checkerboard_.reverse_move(move);
			redo_moves_.push_back(move);    // update for potential redo
			previous_moves_.pop_back();
			switch_players();
		}
	}
}


void Checkers::redo() {
	if (!redo_moves_.empty()) {
		// if single player game, redo 2 moves to avoid changing ai recalculating
		int n_moves_to_redo{ (opponent_.is_user_controlled) ? 1 : 2 };
		for (int i = 0; i < n_moves_to_redo; ++i) {
			make_move(redo_moves_.front(), false);
			redo_moves_.erase(redo_moves_.begin());
		}
	}
}


std::string Checkers::ai_input() {
	return "r";
}
