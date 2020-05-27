#include "checkers.h"



bool isInt(const std::string& s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}



// pieces are crowned if they aren't already kings and can reach the top/bottom of the board
bool isCrowningMove(char pieceChar, BoardPosition boardPosition) {
	bool isKing{ pieceChar == toupper(pieceChar) };
	bool isKingRow{ boardPosition.row == 0 || boardPosition.row == constants::kBoardSize - 1 };

	return !isKing && isKingRow;
}


std::vector<Move> pieceMoves(Checkerboard board, CheckersPlayer opponent,
	BoardPosition piecePosition, std::vector<int> rowMoves, bool canCapture) {

	std::vector<Move> moves;
	int moveDistance{ (canCapture) ? constants::kJumpDistance : constants::kStepDistance };

	for (int rowMove : rowMoves) {
		for (int columnMove : constants::kColumnMoves) {
			BoardPosition movePosition{ piecePosition.row + rowMove * moveDistance, piecePosition.column + columnMove * moveDistance };
			// can only move to open squares
			bool moveAvailable{ board.getPiece(movePosition) == constants::kOpening };

			bool captureAvailable{ true };    // initialized to true to simplify if statement
			BoardPosition capturePosition;
			if (canCapture) {
				capturePosition.row = piecePosition.row + rowMove * constants::kCaptureDistance;
				capturePosition.column = piecePosition.column + columnMove * constants::kCaptureDistance;
				// can only capture if opponent's piece in capturePosition
				captureAvailable = opponent.hasPiece(board.getPiece(capturePosition));
			}

			if (moveAvailable && captureAvailable) {
				Move currentMove{ piecePosition, { movePosition }, isCrowningMove(board.getPiece(piecePosition), movePosition) };

				if (canCapture) {
					currentMove.capturedPieces = { Piece{capturePosition, board.getPiece(capturePosition)} };

					// for jumps, have to continue jumping until no more jumps available
					Checkerboard afterMoveBoard = board;
					afterMoveBoard.executeMove(currentMove);

					// use recursion to continue jump
					std::vector<Move> continueJump{ pieceMoves(afterMoveBoard, opponent, movePosition, rowMoves, canCapture) };

					for (Move jump : continueJump) {
						currentMove.landingPositions.insert(
							currentMove.landingPositions.end(),
							jump.landingPositions.begin(),
							jump.landingPositions.end());

						currentMove.capturedPieces.insert(
							currentMove.capturedPieces.end(),
							jump.capturedPieces.begin(),
							jump.capturedPieces.end());

						currentMove.isCrowning = jump.isCrowning;
					}
				}

				moves.push_back(currentMove);
			}
		}
	}

	return moves;
}


std::vector<Move> boardMoves(Checkerboard board, CheckersPlayer currentPlayer, CheckersPlayer opponent) {
	std::vector<Move> jumps{};
	std::vector<Move> steps{};

	for (BoardPosition position : board.getPlayerPositions(currentPlayer)) {
		char pieceChar = board.getPiece(position);
		std::vector<int> rowMoves;

		if (pieceChar == toupper(pieceChar)) {
			rowMoves = { -1, 1 };    // kings can move up or down
		}
		else {
			rowMoves = { currentPlayer.verticalDirection };    // regular pieces can only move in their player's direction
		}

		std::vector<Move> pieceJumps{ pieceMoves(board, opponent, position, rowMoves, true) };
		jumps.insert(jumps.end(), pieceJumps.begin(), pieceJumps.end());

		// jumps are forced so only need to look for steps if no jumps are available
		if (jumps.empty()) {
			std::vector<Move> pieceSteps{ pieceMoves(board, opponent, position, rowMoves, false) };
			steps.insert(steps.end(), pieceSteps.begin(), pieceSteps.end());
		}
	}

	return (jumps.empty()) ? steps : jumps;
}



Checkers::Checkers() {
	inputMap["q"] = std::bind(&Checkers::quit, this);
	inputMap["quit"] = std::bind(&Checkers::quit, this);
	inputMap["exit"] = std::bind(&Checkers::quit, this);
	
	inputMap["r"] = std::bind(&Checkers::randomMove, this);
	inputMap["rand"] = std::bind(&Checkers::randomMove, this);
	inputMap["random"] = std::bind(&Checkers::randomMove, this);
	
	inputMap["u"] = std::bind(&Checkers::undo, this); 
	inputMap["z"] = std::bind(&Checkers::undo, this);
	inputMap["undo"] = std::bind(&Checkers::undo, this);

	inputMap["y"] = std::bind(&Checkers::redo, this);
	inputMap["redo"] = std::bind(&Checkers::redo, this);

	// change this to new game screen when have states
	inputMap["n"] = std::bind(&Checkers::play, this);
	inputMap["new"] = std::bind(&Checkers::play, this);
	inputMap["newgame"] = std::bind(&Checkers::play, this);
	inputMap["new game"] = std::bind(&Checkers::play, this);
}


std::vector<Move> Checkers::getAvailableMoveList() {
	return availableMoveList_;
}


void Checkers::play() {
	reset();

	while (!availableMoveList_.empty()) {
		render();
		std::string input;
		if (currentPlayer_.isUserControlled) {
			input = getUserInput();
		}
		else {
			input = aiInput();
		}
		processInput(input);
		update();
	}
}


void Checkers::reset() {
	checkerboard_.reset();
	currentPlayer_ = players_[0];
	opponent_ = players_[1];
	previousMoveList_.clear();
	redoMoveList_.clear();
	update();
}


void Checkers::update() {
	availableMoveList_ = boardMoves(checkerboard_, currentPlayer_, opponent_);
}


void Checkers::quit() {
	exit(0);
}


void Checkers::switchPlayers() {
	std::swap(currentPlayer_, opponent_);
}


void Checkers::render() {
	//TODO split this into renderBoard and renderOptions
	std::cout << std::endl;
	std::cout << checkerboard_ << std::endl;

	int i{ 1 };
	for (Move move : availableMoveList_) {
		std::cout << i++ << ") ";
		std::cout << "(" << move.startPosition.row << ", " << move.startPosition.column << ")";
		for (BoardPosition landingPosition : move.landingPositions) {
			std::cout << " to (" << landingPosition.row << ", " << landingPosition.column << ")";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


std::string Checkers::getUserInput() {
	std::cin.clear();
	std::string userInput;
	std::getline(std::cin, userInput);
	
	// convert to lower case
	std::transform(userInput.begin(), userInput.end(), userInput.begin(), [](unsigned char c) { return tolower(c); });
	
	return userInput;
}


void Checkers::processInput(std::string input) {
	if (inputMap.count(input) == 1) {
		// pre-defined allowable input
		inputMap[input]();
	}
	else {
		if (isInt(input)) {
			int moveIndex{ std::stoi(input) - 1};
			if (-1 < moveIndex && moveIndex < availableMoveList_.size()) {
				
				// number representing which available move to make
				makeMove(availableMoveList_[moveIndex], true);
			}
		}
	}
}


void Checkers::randomMove() {
	makeMove(availableMoveList_[rand() % availableMoveList_.size()], true);
}


void Checkers::makeMove(Move move, bool isNewMove) {
	checkerboard_.executeMove(move);
	previousMoveList_.push_back(move);    // update for potential undo
	if (isNewMove) redoMoveList_.clear();    // if not a redo, need to clear the redo list
	switchPlayers();
}


void Checkers::undo() {
	// if single player game, need to reverse 2 moves to get back to user's last play
	int movesToReverse{ (opponent_.isUserControlled) ? 1 : 2 };

	if (movesToReverse <= previousMoveList_.size()) {
		for (int i = 0; i < movesToReverse; ++i) {
			Move move{ previousMoveList_.back() };
			checkerboard_.reverseMove(move);
			redoMoveList_.push_back(move);    // update for potential redo
			previousMoveList_.pop_back();
			switchPlayers();
		}
	}
}


void Checkers::redo() {
	if (!redoMoveList_.empty()) {
		// if single player game, redo 2 moves to avoid changing ai recalculating
		int movesToRedo{ (opponent_.isUserControlled) ? 1 : 2 };
		for (int i = 0; i < movesToRedo; ++i) {
			makeMove(redoMoveList_.front(), false);
			redoMoveList_.erase(redoMoveList_.begin());
		}
	}
}


std::string Checkers::aiInput() {
	return "r";
}
