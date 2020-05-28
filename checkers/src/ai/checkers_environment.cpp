#include "checkers_environment.h"
#include <iostream>


std::vector<std::vector<std::string>> pieceMoves(
	std::vector<std::string> startBoard, std::vector<int> rowSteps, int row, int column, int newDistance, int captureDistance) {

	std::vector<std::vector<std::string>> moveBoards, continueBoards;
	std::vector<std::string> currentBoard;
	int captureRow{ 0 }, captureColumn{ 0 }, newRow{ 0 }, newColumn{ 0 };
	char piece = startBoard[row][column], capturePiece;

	for (int rowStep : rowSteps) {
		for (int columnStep : kColumnSteps) {
			currentBoard = startBoard;

			captureRow = row + rowStep * captureDistance;
			captureColumn = column + columnStep * captureDistance;
			capturePiece = tolower(currentBoard[captureRow][captureColumn]);
			newRow = row + rowStep * newDistance;
			newColumn = column + columnStep * newDistance;

			if (currentBoard[newRow][newColumn] == '-' && (captureDistance == 0 ||
				(capturePiece != tolower(piece) && capturePiece != '-'))) {

				currentBoard[newRow][newColumn] = (newRow == 2 || newRow == startBoard.size() - 3) ? toupper(piece) : piece;
				currentBoard[row][column] = '-';

				if (captureDistance != 0) {
					currentBoard[captureRow][captureColumn] = '-';
					continueBoards = pieceMoves(currentBoard, rowSteps, newRow, newColumn, newDistance, captureDistance);
				}

				if (continueBoards.empty()) {
					moveBoards.push_back(currentBoard);
				}
				else {
					moveBoards.insert(moveBoards.begin(), continueBoards.begin(), continueBoards.end());
				}
			}
		}
	}

	return moveBoards;
}



std::vector<std::vector<std::string>> boardActions(std::vector<std::string> startBoard, char player) {
	std::vector<std::vector<std::string>> stepBoards, jumpBoards, pieceSteps, pieceJumps;
	std::vector<int> rowSteps;
	char piece;

	for (size_t row = 2; row < startBoard.size() - 2; ++row) {
		for (size_t column = 2 + (row + 1) % 2; column < startBoard[row].size() - 2; column += 2) {
			piece = startBoard[row][column];
			if (tolower(piece) == player) {
				if (piece == 'r') rowSteps = { -kStepDistance };
				else if (piece == 'w') rowSteps = { kStepDistance };
				else rowSteps = { -kStepDistance, kStepDistance };

				pieceJumps = pieceMoves(startBoard, rowSteps, row, column, kJumpDistance, kJumpDistance / 2);
				jumpBoards.insert(jumpBoards.end(), pieceJumps.begin(), pieceJumps.end());

				if (jumpBoards.empty()) {
					pieceSteps = pieceMoves(startBoard, rowSteps, row, column, kStepDistance, 0);
					stepBoards.insert(stepBoards.end(), pieceSteps.begin(), pieceSteps.end());
				}
			}
		}
	}

	return (jumpBoards.empty()) ? stepBoards : jumpBoards;
}


std::vector<std::shared_ptr<Player>> CheckersEnvironment::getPlayers() {
	return players_;
}


//State* CheckersEnvironment::reset() {
std::shared_ptr<State> CheckersEnvironment::reset() {
	nPlays_ = 0;
	//state_ = new State{ kDefaultBoard, 0, false, players_[0] };
	state_ = std::make_shared<State>(State{ kDefaultBoard, 0, false, players_[0] });
	return state_;
}


//State* CheckersEnvironment::step(Action* action) {
std::shared_ptr<State> CheckersEnvironment::step(std::shared_ptr<Action> action) {
	std::vector<std::string> observation = std::any_cast<std::vector<std::string>>(action->action);

	for (auto row : observation) {
		std::cout << row << std::endl;
	}

	++nPlays_;
	state_ = step(state_, action);

	return state_;
}


//State* CheckersEnvironment::step(State* state, Action* action) {
std::shared_ptr<State> CheckersEnvironment::step(std::shared_ptr<State> state, std::shared_ptr<Action> action) {
	std::vector<std::string> observation = std::any_cast<std::vector<std::string>>(action->action);

	double reward = 0;
	//Player* opposingPlayer = opponent(state->currentPlayer);
	std::shared_ptr<Player> opposingPlayer = opponent(state->currentPlayer);
	char nextPlayerChar = std::any_cast<char>(opposingPlayer->player);
	std::vector<std::vector<std::string>> opponentActions{ boardActions(observation, nextPlayerChar) };
	if (opponentActions.size() == 0) reward = (nextPlayerChar == 'r') ? -1 : 1;

	bool isTerminal = reward != 0 || nPlays_ > kMaxPlays;

	//return new State{ observation, reward, isTerminal, opposingPlayer };
	return std::make_shared<State>(State{ observation, reward, isTerminal, opposingPlayer });
}


//std::vector<Action*> CheckersEnvironment::getActions() {
std::vector<std::shared_ptr<Action>> CheckersEnvironment::getActions() {
	return getActions(state_);
}


//std::vector<Action*> CheckersEnvironment::getActions(State* state) {
std::vector<std::shared_ptr<Action>> CheckersEnvironment::getActions(std::shared_ptr<State> state) {
	std::vector<std::shared_ptr<Action>> actions;
	std::vector<std::string> currentBoard{ convertStateToBoard(state) };
	char player{ std::any_cast<char>(state->currentPlayer->player) };

	for (std::vector<std::string> board : boardActions(currentBoard, player)) {
		//actions.push_back(new Action{ board });
		actions.push_back(std::make_shared<Action>(Action{ board }));
	}

	return actions;
}


//std::vector<double> CheckersEnvironment::featurize(State* state) {
std::vector<double> CheckersEnvironment::featurize(std::shared_ptr<State> state) {
	std::vector<double> features;
	std::vector<std::string> board{ convertStateToBoard(state) };
	for (size_t row = 2; row < board.size() - 2; ++row) {
		for (size_t column = 2 + (row + 1) % 2; column < board[row].size() - 2; column += 2) {
			for (char piece : {'r', 'R', 'w', 'W'}) {
				features.push_back((board[row][column] == piece) ? 1 : 0);
			}
		}
	}
	return features;
}


//std::vector<std::string> CheckersEnvironment::convertStateToBoard(State* state) {
std::vector<std::string> CheckersEnvironment::convertStateToBoard(std::shared_ptr<State> state) {
	return std::any_cast<std::vector<std::string>>(state->observation);
}


//Player* CheckersEnvironment::opponent(Player* currentPlayer) {
std::shared_ptr<Player> CheckersEnvironment::opponent(std::shared_ptr<Player> currentPlayer) {
	return (currentPlayer == players_[0]) ? players_[1] : players_[0];
}
