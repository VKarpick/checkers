#include "checkers_environment.h"


State* CheckersEnvironment::reset() {
	play_counter_ = 0;
	current_player_index_ = 0;
	return new State(kDefaultBoard, 0, false, players_[current_player_index_]);
}

State* CheckersEnvironment::step(Action* action) {
	return new State(0, 0, false);
}

State* CheckersEnvironment::step(State* state, Action* action) {
	return new State(0, 0, false);
}

std::vector<Action*> CheckersEnvironment::actions() {
	return { new Action() };
}

std::vector<Action*> CheckersEnvironment::actions(State* state) {
	return { new Action() };
}

std::vector<double> CheckersEnvironment::featurize(State* state) {
	return { 0 };
}

////char CheckersEnv::Opponent(char player) { return (player == 'r') ? 'w' : 'r'; }
////char CheckersEnv::Opponent() { return Opponent(current_player_); }
////
////vector<int> CheckersEnv::BoardToFeatures(vector<string> board_to_convert) {
////	// feature for player?
////	// feature for blank square?
////	vector<int> features;
////	for (size_t row = 2; row < board_to_convert.size() - 2; ++row) {
////		for (size_t column = 2 + (row + 1) % 2; column < board_to_convert[row].size() - 2; column += 2) {
////			for (char piece : {'r', 'R', 'w', 'W'}) {
////				features.push_back((board_to_convert[row][column] == piece) ? 1 : 0);
////			}
////		}
////	}
////	return features;
////}
////
////vector<int> CheckersEnv::BoardToFeatures() { return BoardToFeatures(board_); }
////
////vector<string> CheckersEnv::Reset() {
////	board_ = kDefaultBoard;
////	current_player_ = 'r';
////	play_counter_ = 0;
////	return board_;
////}
////
////double CheckersEnv::GetReward(vector<string> current_board, int n_plays, char player) {
////	//// too many plays, call it a draw with slight penalty
////	//if (n_plays > max_plays) return -1.0;
////
////	// opponent can't make a play, reward the winner
////	if (actions(current_board, Opponent(player)).empty()) return (player == 'r') ? 1.0 : -1.0;
////
////	// game continues, no reward
////	return 0;
////}
////
////double CheckersEnv::GetReward() { return GetReward(board_, play_counter_, current_player_); }
////
////tuple<vector<string>, double, bool> CheckersEnv::Step(vector<string> action_board) {
////	board_ = action_board;
////	++play_counter_;
////	double reward = GetReward();
////	current_player_ = Opponent();
////	return make_tuple(board_, reward, reward != 0 || play_counter_ > kMaxPlays);
////}
////
////vector<vector<string>> CheckersEnv::PieceMoves(
////	vector<string> start_board, vector<int> row_steps, int row, int column, int new_distance, int capture_distance) {
////
////	vector<vector<string>> move_boards, continue_boards;
////	vector<string> current_board;
////	int capture_row{ 0 }, capture_column{ 0 }, new_row{ 0 }, new_column{ 0 };
////	char piece = start_board[row][column], capture_piece;
////
////	for (int row_step : row_steps) {
////		for (int column_step : kColumnSteps) {
////			current_board = start_board;
////
////			capture_row = row + row_step * capture_distance;
////			capture_column = column + column_step * capture_distance;
////			capture_piece = tolower(current_board[capture_row][capture_column]);
////			new_row = row + row_step * new_distance;
////			new_column = column + column_step * new_distance;
////
////			if (current_board[new_row][new_column] == '-' && (capture_distance == 0 ||
////				(capture_piece != tolower(piece) && capture_piece != '-'))) {
////
////				current_board[new_row][new_column] = (new_row == 2 || new_row == board_.size() - 3) ? toupper(piece) : piece;
////				current_board[row][column] = '-';
////
////				if (capture_distance != 0) {
////					current_board[capture_row][capture_column] = '-';
////					continue_boards = PieceMoves(current_board, row_steps, new_row, new_column, new_distance, capture_distance);
////				}
////
////				if (continue_boards.empty()) {
////					move_boards.push_back(current_board);
////				}
////				else {
////					move_boards.insert(move_boards.begin(), continue_boards.begin(), continue_boards.end());
////				}
////			}
////		}
////	}
////
////	return move_boards;
////}
////
////
////vector<vector<string>> CheckersEnv::actions(vector<string> start_board, char player) {
////	vector<vector<string>> step_boards, jump_boards, piece_steps, piece_jumps;
////	vector<int> row_steps;
////	char piece;
////
////	for (size_t row = 2; row < start_board.size() - 2; ++row) {
////		for (size_t column = 2 + (row + 1) % 2; column < start_board[row].size() - 2; column += 2) {
////			piece = start_board[row][column];
////			if (tolower(piece) == player) {
////				if (piece == 'r') row_steps = { -kStepDistance };
////				else if (piece == 'w') row_steps = { kStepDistance };
////				else row_steps = { -kStepDistance, kStepDistance };
////
////				piece_jumps = PieceMoves(start_board, row_steps, row, column, kJumpDistance, kJumpDistance / 2);
////				jump_boards.insert(jump_boards.end(), piece_jumps.begin(), piece_jumps.end());
////
////				if (jump_boards.empty()) {
////					piece_steps = PieceMoves(start_board, row_steps, row, column, kStepDistance, 0);
////					step_boards.insert(step_boards.end(), piece_steps.begin(), piece_steps.end());
////				}
////			}
////		}
////	}
////
////	return (jump_boards.empty()) ? step_boards : jump_boards;
////}
////
////vector<vector<string>> CheckersEnv::actions() { return actions(board_, current_player_); }
