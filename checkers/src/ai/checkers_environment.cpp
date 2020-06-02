#include "checkers_environment.h"



std::vector<std::shared_ptr<Player>> CheckersEnvironment::get_players() {
	return players_;
}


std::shared_ptr<State> CheckersEnvironment::reset() {
	n_plays_ = 0;
	state_ = std::make_shared<State>(State{ Checkerboard(constants::starting_board), 0, false, players_[0] });
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

	double reward{ 0 };

	CheckersPlayer current_player{ checkers_player_from_state(state) };
	std::shared_ptr<Player> opponent{ opponent_from_state(state) };
	std::vector<Move> opponent_actions{ CheckerboardMoves::board_moves(checkerboard, checkers_player_from_player(opponent), current_player) };

	if (opponent_actions.size() == 0) {
		//TODO remove magic variable
		reward = (current_player.player == 'r') ? 1 : -1;
	}

	bool is_terminal{ reward != 0 || n_plays_ > max_plays };

	return std::make_shared<State>(State{ checkerboard, reward, is_terminal, opponent });
}


std::vector<std::shared_ptr<Action>> CheckersEnvironment::get_actions() {
	return get_actions(state_);
}


std::vector<std::shared_ptr<Action>> CheckersEnvironment::get_actions(std::shared_ptr<State> state) {
	std::vector<std::shared_ptr<Action>> actions;
	Checkerboard checkerboard{ checkerboard_from_state(state) };
	CheckersPlayer current_player{ checkers_player_from_state(state) };
	CheckersPlayer opponent{ checkers_player_from_player(opponent_from_state(state)) };
	
	for (Move move : CheckerboardMoves::board_moves(checkerboard, current_player, opponent)) {
		actions.push_back(std::make_shared<Action>(Action{ move }));
	}

	return actions;
}


std::vector<double> CheckersEnvironment::featurize(std::shared_ptr<State> state) {
	std::vector<double> features;
	std::vector<std::string> board{ checkerboard_from_state(state).get_board() };
	
	for (size_t row = 0; row < board.size(); ++row) {
		for (size_t column = 0 + (row + 1) % 2; column < board[row].size(); column += 2) {
			//TODO remove magic variables
			for (char piece : {'r', 'w'}) {
				features.push_back((board[row][column] == piece) ? 1 : 0);    // feature for regular pieces
				features.push_back((board[row][column] == toupper(piece)) ? 1 : 0);    // feature for kings
			}
		}
	}

	return features;
}


Checkerboard CheckersEnvironment::checkerboard_from_state(std::shared_ptr<State> state) {
	return std::any_cast<Checkerboard>(state->observation);
}


std::shared_ptr<Player> CheckersEnvironment::opponent_from_state(std::shared_ptr<State> state) {
	return (state->current_player == players_[0]) ? players_[1] : players_[0];
}


CheckersPlayer CheckersEnvironment::checkers_player_from_state(std::shared_ptr<State> state) {
	return checkers_player_from_player(state->current_player);
}


CheckersPlayer CheckersEnvironment::checkers_player_from_player(std::shared_ptr<Player> player) {
	return std::any_cast<CheckersPlayer>(player->player);
}
