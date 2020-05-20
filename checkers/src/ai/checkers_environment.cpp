#include "checkers_environment.h"


std::vector<std::vector<std::string>> piece_moves(
	std::vector<std::string> start_board, std::vector<int> row_steps, int row, int column, int new_distance, int capture_distance) {

	std::vector<std::vector<std::string>> move_boards, continue_boards;
	std::vector<std::string> current_board;
	int capture_row{ 0 }, capture_column{ 0 }, new_row{ 0 }, new_column{ 0 };
	char piece = start_board[row][column], capture_piece;

	for (int row_step : row_steps) {
		for (int column_step : kColumnSteps) {
			current_board = start_board;

			capture_row = row + row_step * capture_distance;
			capture_column = column + column_step * capture_distance;
			capture_piece = tolower(current_board[capture_row][capture_column]);
			new_row = row + row_step * new_distance;
			new_column = column + column_step * new_distance;

			if (current_board[new_row][new_column] == '-' && (capture_distance == 0 ||
				(capture_piece != tolower(piece) && capture_piece != '-'))) {

				current_board[new_row][new_column] = (new_row == 2 || new_row == start_board.size() - 3) ? toupper(piece) : piece;
				current_board[row][column] = '-';

				if (capture_distance != 0) {
					current_board[capture_row][capture_column] = '-';
					continue_boards = piece_moves(current_board, row_steps, new_row, new_column, new_distance, capture_distance);
				}

				if (continue_boards.empty()) {
					move_boards.push_back(current_board);
				}
				else {
					move_boards.insert(move_boards.begin(), continue_boards.begin(), continue_boards.end());
				}
			}
		}
	}

	return move_boards;
}


std::vector<std::vector<std::string>> board_actions(std::vector<std::string> start_board, char player) {
	std::vector<std::vector<std::string>> step_boards, jump_boards, piece_steps, piece_jumps;
	std::vector<int> row_steps;
	char piece;

	for (size_t row = 2; row < start_board.size() - 2; ++row) {
		for (size_t column = 2 + (row + 1) % 2; column < start_board[row].size() - 2; column += 2) {
			piece = start_board[row][column];
			if (tolower(piece) == player) {
				if (piece == 'r') row_steps = { -kStepDistance };
				else if (piece == 'w') row_steps = { kStepDistance };
				else row_steps = { -kStepDistance, kStepDistance };

				piece_jumps = piece_moves(start_board, row_steps, row, column, kJumpDistance, kJumpDistance / 2);
				jump_boards.insert(jump_boards.end(), piece_jumps.begin(), piece_jumps.end());

				if (jump_boards.empty()) {
					piece_steps = piece_moves(start_board, row_steps, row, column, kStepDistance, 0);
					step_boards.insert(step_boards.end(), piece_steps.begin(), piece_steps.end());
				}
			}
		}
	}

	return (jump_boards.empty()) ? step_boards : jump_boards;
}


State* CheckersEnvironment::reset() {
	play_counter_ = 0;
	current_player_index_ = 0;
	state_ = new State{ kDefaultBoard, 0, false, players_[current_player_index_] };
	return state_;
}

State* CheckersEnvironment::step(Action* action) {
	++play_counter_;
	current_player_index_ = (current_player_index_ + 1) % 2;
	state_ = step(state_, action);
	return state_;
}

State* CheckersEnvironment::step(State* state, Action* action) {
	std::vector<std::string> observation = std::any_cast<std::vector<std::string>>(action->action);

	double reward = 0;
	char next_player_char = std::any_cast<char>(players_[current_player_index_]);
	std::vector<std::vector<std::string>> opponent_actions{ board_actions(observation, next_player_char) };
	if (opponent_actions.size() == 0) reward = (next_player_char == 'r') ? -1 : 1;

	bool terminal = reward != 0 || play_counter_ > kMaxPlays;
	Player* current_player = opponent(state->current_player);

	return new State{ observation, reward, terminal, current_player };
}

std::vector<Action*> CheckersEnvironment::actions() {
	return actions(state_);
}

std::vector<Action*> CheckersEnvironment::actions(State* state) {
	std::vector<Action*> action_vector;
	std::vector<std::string> current_board{ board_from_state(state) };
	char player{ std::any_cast<char>(state->current_player->player) };

	for (std::vector<std::string> board : board_actions(current_board, player)) {
		action_vector.push_back(new Action{ board });
	}

	return action_vector;
}

std::vector<double> CheckersEnvironment::featurize(State* state) {
	std::vector<double> features;
	std::vector<std::string> board{ board_from_state(state) };
	for (size_t row = 2; row < board.size() - 2; ++row) {
		for (size_t column = 2 + (row + 1) % 2; column < board[row].size() - 2; column += 2) {
			for (char piece : {'r', 'R', 'w', 'W'}) {
				features.push_back((board[row][column] == piece) ? 1 : 0);
			}
		}
	}
	return features;
}

std::vector<std::string> CheckersEnvironment::board_from_state(State* state) {
	return std::any_cast<std::vector<std::string>>(state->observation);
}

Player* CheckersEnvironment::opponent(Player* current_player) {
	return (state_->current_player == players_[0]) ? players_[1] : players_[0];
}
