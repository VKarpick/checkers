#include "checkers.h"



bool is_int(const std::string& s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}



Checkers::Checkers() {
	srand(time(0));

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

	input_map["n"] = std::bind(&Checkers::new_game, this);
	input_map["new"] = std::bind(&Checkers::new_game, this);
	input_map["newgame"] = std::bind(&Checkers::new_game, this);
	input_map["new game"] = std::bind(&Checkers::new_game, this);
}


std::vector<Move> Checkers::get_available_moves() {
	return available_moves_;
}


void Checkers::start() {
	while (state_ != CheckersState::Exiting) {
		game_loop();
	}
	exit(0);
}


void Checkers::game_loop() {
	switch (state_) {
		case CheckersState::StartScreen: {
			render_start_screen();
			break;
		}
		case CheckersState::Playing: {
			update();
			render();

			if (available_moves_.empty()) {
				state_ = CheckersState::EndScreen;
			}
			else {
				process_input();
			}
			
			break;
		}
		case CheckersState::EndScreen: {
			render_end_screen();
			break;
		}
	}
}


void Checkers::render_start_screen() {
	std::string user_input;
	std::vector<std::string> ordinal{ "first", "second" };
	std::vector<int> directions{ -1, 1 };
	players_.clear();
	while (players_.size() < 2) {
		std::string player_color{ constants::player_colors[players_.size()] };
		std::cout << "User controlling " << ordinal[players_.size()] << " player (" << player_color << ")? (y/n) ";
		user_input = get_user_input();

		if (user_input == "yes") { user_input = "y"; }
		if (user_input == "no") { user_input = "n"; }

		if (user_input == "y" || user_input == "n") {
			char piece{ constants::pieces[players_.size()] };
			int direction{ directions[players_.size()] };
			players_.push_back(CheckersPlayer{ piece, direction, user_input == "y" });
		}

		if (user_input == "q" || user_input == "quit") {
			quit();
			return;
		}
	}
	reset();
	state_ = CheckersState::Playing;
}


void Checkers::render_end_screen() {
	int winner{ (players_[0] == current_player_) ? 1 : 0 };
	if (players_[0].is_user_controlled == players_[1].is_user_controlled) {
		std::cout << constants::player_colors[winner] << " wins!" << std::endl;
	}
	else {
		std::string won_lost{ (players_[winner].is_user_controlled) ? " won" : "lost" };
		std::cout << "Looks like you " << won_lost << " that one." << std::endl;
	}

	std::string user_input;
	while (state_ == CheckersState::EndScreen) {
		std::cout << "\nPlay again?" << std::endl;
		std::cout << "0) Yes" << std::endl;
		std::cout << "1) Change settings" << std::endl;
		std::cout << "2) No" << std::endl;
		user_input = get_user_input();

		if (user_input == "0" || user_input == "y" || user_input == "yes") {
			reset();
			state_ = CheckersState::Playing;
		}
		else if (user_input == "1" || user_input == "change" || user_input == "change settings") {
			state_ = CheckersState::StartScreen;
		}
		else if (user_input == "2" || user_input == "n" || user_input == "no") {
			state_ = CheckersState::Exiting;
		}
		std::cout << std::endl;
	}
}


void Checkers::reset() {
	checkerboard_.reset();
	current_player_ = players_[0];
	opponent_ = players_[1];
	previous_moves_.clear();
	redo_moves_.clear();
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


void Checkers::new_game() {
	state_ = CheckersState::StartScreen;
}


void Checkers::quit() {
	state_ = CheckersState::Exiting;
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


void Checkers::process_input() {
	std::string input;
	if (current_player_.is_user_controlled) {
		input = get_user_input();
	}
	if (!current_player_.is_user_controlled || input == "a") {
		input = ai_input();
	}

	process_input(input);
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
