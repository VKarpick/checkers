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

	//TODO remove this
	Checkerboard checkerboard{ checkerboard_from_state(state_) };
	std::cout << checkerboard << std::endl;

	return state_;
}


std::shared_ptr<State> CheckersEnvironment::step(std::shared_ptr<State> state, std::shared_ptr<Action> action) {
	Checkerboard checkerboard{ checkerboard_from_state(state) };
	checkerboard.execute_move(std::any_cast<Move>(action->action));

	double reward{ 0 };

	CheckersPlayer current_player{ checkers_player_from_state(state) };
	std::shared_ptr<Player> opponent{ opponent_from_state(state) };
	std::vector<Move> opponent_actions{ board_moves(checkerboard, checkers_player_from_player(opponent), current_player) };

	if (opponent_actions.size() == 0) {
		reward = (current_player.player_char == 'r') ? 1 : -1;
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
	
	for (Move move : board_moves(checkerboard, current_player, opponent)) {
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








//std::vector<std::vector<std::string>> pieceMoves(
//	std::vector<std::string> startBoard, std::vector<int> rowSteps, int row, int column, int newDistance, int captureDistance) {
//
//	std::vector<std::vector<std::string>> moveBoards, continueBoards;
//	std::vector<std::string> currentBoard;
//	int captureRow{ 0 }, captureColumn{ 0 }, newRow{ 0 }, newColumn{ 0 };
//	char piece = startBoard[row][column], capturePiece;
//
//	for (int rowStep : rowSteps) {
//		for (int columnStep : kColumnSteps) {
//			currentBoard = startBoard;
//
//			captureRow = row + rowStep * captureDistance;
//			captureColumn = column + columnStep * captureDistance;
//			capturePiece = tolower(currentBoard[captureRow][captureColumn]);
//			newRow = row + rowStep * newDistance;
//			newColumn = column + columnStep * newDistance;
//
//			if (currentBoard[newRow][newColumn] == '-' && (captureDistance == 0 ||
//				(capturePiece != tolower(piece) && capturePiece != '-'))) {
//
//				currentBoard[newRow][newColumn] = (newRow == 2 || newRow == startBoard.size() - 3) ? toupper(piece) : piece;
//				currentBoard[row][column] = '-';
//
//				if (captureDistance != 0) {
//					currentBoard[captureRow][captureColumn] = '-';
//					continueBoards = pieceMoves(currentBoard, rowSteps, newRow, newColumn, newDistance, captureDistance);
//				}
//
//				if (continueBoards.empty()) {
//					moveBoards.push_back(currentBoard);
//				}
//				else {
//					moveBoards.insert(moveBoards.begin(), continueBoards.begin(), continueBoards.end());
//				}
//			}
//		}
//	}
//
//	return moveBoards;
//}
//
//
//
//std::vector<std::vector<std::string>> boardActions(std::vector<std::string> startBoard, char player) {
//	std::vector<std::vector<std::string>> stepBoards, jumpBoards, pieceSteps, pieceJumps;
//	std::vector<int> rowSteps;
//	char piece;
//
//	for (size_t row = 2; row < startBoard.size() - 2; ++row) {
//		for (size_t column = 2 + (row + 1) % 2; column < startBoard[row].size() - 2; column += 2) {
//			piece = startBoard[row][column];
//			if (tolower(piece) == player) {
//				if (piece == 'r') rowSteps = { -kStepDistance };
//				else if (piece == 'w') rowSteps = { kStepDistance };
//				else rowSteps = { -kStepDistance, kStepDistance };
//
//				pieceJumps = pieceMoves(startBoard, rowSteps, row, column, kJumpDistance, kJumpDistance / 2);
//				jumpBoards.insert(jumpBoards.end(), pieceJumps.begin(), pieceJumps.end());
//
//				if (jumpBoards.empty()) {
//					pieceSteps = pieceMoves(startBoard, rowSteps, row, column, kStepDistance, 0);
//					stepBoards.insert(stepBoards.end(), pieceSteps.begin(), pieceSteps.end());
//				}
//			}
//		}
//	}
//
//	return (jumpBoards.empty()) ? stepBoards : jumpBoards;
//}
//
//
//std::vector<std::shared_ptr<Player>> CheckersEnvironment::get_players() {
//	return players_;
//}
//
//
//std::shared_ptr<State> CheckersEnvironment::reset() {
//	nPlays_ = 0;
//	state_ = std::make_shared<State>(State{ kDefaultBoard, 0, false, players_[0] });
//	return state_;
//}
//
//
//std::shared_ptr<State> CheckersEnvironment::step(std::shared_ptr<Action> action) {
//	std::vector<std::string> observation = std::any_cast<std::vector<std::string>>(action->action);
//
//	/*for (auto row : observation) {
//		std::cout << row << std::endl;
//	}*/
//
//	++nPlays_;
//	state_ = step(state_, action);
//
//	return state_;
//}
//
//
//std::shared_ptr<State> CheckersEnvironment::step(std::shared_ptr<State> state, std::shared_ptr<Action> action) {
//	std::vector<std::string> observation = std::any_cast<std::vector<std::string>>(action->action);
//
//	double reward = 0;
//	std::shared_ptr<Player> opposingPlayer = opponent(state->current_player);
//	char nextPlayerChar = std::any_cast<char>(opposingPlayer->player);
//	std::vector<std::vector<std::string>> opponentActions{ boardActions(observation, nextPlayerChar) };
//	if (opponentActions.size() == 0) reward = (nextPlayerChar == 'r') ? -1 : 1;
//
//	bool isTerminal = reward != 0 || nPlays_ > kMaxPlays;
//
//	return std::make_shared<State>(State{ observation, reward, isTerminal, opposingPlayer });
//}
//
//
//std::vector<std::shared_ptr<Action>> CheckersEnvironment::get_actions() {
//	return get_actions(state_);
//}
//
//
//std::vector<std::shared_ptr<Action>> CheckersEnvironment::get_actions(std::shared_ptr<State> state) {
//	std::vector<std::shared_ptr<Action>> actions;
//	std::vector<std::string> currentBoard{ convert_state_to_board(state) };
//	char player{ std::any_cast<char>(state->current_player->player) };
//
//	for (std::vector<std::string> board : boardActions(currentBoard, player)) {
//		actions.push_back(std::make_shared<Action>(Action{ board }));
//	}
//
//	return actions;
//}
//
//
//std::vector<double> CheckersEnvironment::featurize(std::shared_ptr<State> state) {
//	std::vector<double> features;
//	std::vector<std::string> board{ convert_state_to_board(state) };
//	for (size_t row = 2; row < board.size() - 2; ++row) {
//		for (size_t column = 2 + (row + 1) % 2; column < board[row].size() - 2; column += 2) {
//			for (char piece : {'r', 'R', 'w', 'W'}) {
//				features.push_back((board[row][column] == piece) ? 1 : 0);
//			}
//		}
//	}
//	return features;
//}
//
//
//std::vector<std::string> CheckersEnvironment::convert_state_to_board(std::shared_ptr<State> state) {
//	return std::any_cast<std::vector<std::string>>(state->observation);
//}
//
//
//std::shared_ptr<Player> CheckersEnvironment::opponent(std::shared_ptr<Player> currentPlayer) {
//	return (currentPlayer == players_[0]) ? players_[1] : players_[0];
//}
