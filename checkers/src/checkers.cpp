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
		if (!current_player_.is_user_controlled || input == "a") {
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


void Checkers::render() {
	std::cout << std::endl;
	render_board();

	// only display available moves for user
	if (current_player_.is_user_controlled) {
		render_options();
	}
	std::cout << std::endl;
}


void Checkers::quit() {
	exit(0);
}


void Checkers::render_board() {
	std::cout << checkerboard_ << std::endl;
}


void Checkers::render_options() {
	int i{ 0 };
	for (Move move : available_moves_) {
		std::cout << i++ << ") ";
		std::cout << "(" << move.start_position.row << ", " << move.start_position.column << ")";
		for (BoardPosition landing_position : move.landing_positions) {
			std::cout << " to (" << landing_position.row << ", " << landing_position.column << ")";
		}
		std::cout << std::endl;
	}
}


void Checkers::switch_players() {
	std::swap(current_player_, opponent_);
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
			int move_index{ std::stoi(input) };
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
	Move ai_move{ checkers_ai_.action_selection(checkerboard_, current_player_) };

	for (int i = 0; i < available_moves_.size(); ++i) {
		if (available_moves_[i] == ai_move) {
			return std::to_string(i);
		}
	}

	return "q";
}
