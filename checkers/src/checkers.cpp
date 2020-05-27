#include "checkers.h"



bool isInt(const std::string& s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
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

	//inputMap["y"] = std::bind(&Checkers::redo, this);
	//inputMap["n"] = std::bind(&Checkers::newGame, this);
}


//TODO remove this
//void Checkers::printMoveBoards() { 
//	update();
//	for (Move move : availableMoveList_) {
//		Checkerboard newBoard{ checkerboard_ };
//		newBoard.executeMove(move);
//		std::cout << newBoard << std::endl;
//	}
//}


void Checkers::play() {
	reset();

	while (!availableMoveList_.empty()) {
		render();
		processInput(getUserInput());
		update();
	}
}


void Checkers::reset() {
	checkerboard_.reset();
	currentPlayer_ = players_[0];
	opponent_ = players_[1];
	previousMoveList_.clear();
	update();
}


void Checkers::update() {
	std::vector<Move> jumps{};
	std::vector<Move> steps{};

	for (BoardPosition position : checkerboard_.getPlayerPositions(currentPlayer_)) {
		char pieceChar = checkerboard_.getPiece(position);
		std::vector<int> rowMoves;

		if (pieceChar == toupper(pieceChar)) {
			rowMoves = { -1, 1 };    // kings can move up or down
		}
		else {
			rowMoves = { currentPlayer_.verticalDirection };    // regular pieces can only move in their player's direction
		}

		std::vector<Move> pieceJumps{ pieceMoves(checkerboard_, position, rowMoves, true) };
		jumps.insert(jumps.end(), pieceJumps.begin(), pieceJumps.end());

		// jumps are forced so only need to look for steps if no jumps are available
		if (jumps.empty()) {
			std::vector<Move> pieceSteps{ pieceMoves(checkerboard_, position, rowMoves, false) };
			steps.insert(steps.end(), pieceSteps.begin(), pieceSteps.end());
		}
	}

	availableMoveList_ = (jumps.empty()) ? steps : jumps;
}


void Checkers::quit() {
	exit(0);
}


std::vector<Move> Checkers::pieceMoves(Checkerboard board, BoardPosition piecePosition, std::vector<int> rowMoves, bool canCapture) {
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
				captureAvailable = opponent_.hasPiece(board.getPiece(capturePosition));
			}

			if (moveAvailable && captureAvailable) {
				Move currentMove{ piecePosition, { movePosition }, isCrowningMove(board.getPiece(piecePosition), movePosition) };

				if (canCapture) {
					//currentMove.capturedPositions = { capturePosition };
					currentMove.capturedPieces = { Piece{capturePosition, checkerboard_.getPiece(capturePosition)} };

					// for jumps, have to continue jumping until no more jumps available
					Checkerboard afterMoveBoard = board;
					afterMoveBoard.executeMove(currentMove);

					// use recursion to continue jump
					std::vector<Move> continueJump{ pieceMoves(afterMoveBoard, movePosition, rowMoves, canCapture) };
					
					for (Move jump : continueJump) {
						currentMove.landingPositions.insert(
							currentMove.landingPositions.end(),
							jump.landingPositions.begin(),
							jump.landingPositions.end());

						/*currentMove.capturedPositions.insert(
							currentMove.capturedPositions.end(),
							jump.capturedPositions.begin(),
							jump.capturedPositions.end());*/

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


// pieces are crowned if they aren't already kings and can reach the top/bottom of the board
bool Checkers::isCrowningMove(char pieceChar, BoardPosition boardPosition) {
	bool isKing{ pieceChar == toupper(pieceChar) };
	bool isKingRow{ boardPosition.row == 0 || boardPosition.row == constants::kBoardSize - 1 };

	return !isKing && isKingRow;
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
	//TODO convert input to lowercase
	return userInput;
}


void Checkers::processInput(std::string input) {
	if (inputMap.count(input) == 1) {
		inputMap[input]();
	}
	else {
		if (isInt(input)) {
			int moveIndex{ std::stoi(input) - 1};
			if (-1 < moveIndex && moveIndex < availableMoveList_.size()) {
				makeMove(availableMoveList_[moveIndex]);
				/*checkerboard_.executeMove(availableMoveList_[moveIndex]);
				switchPlayers();*/
			}
		}
	}
}


void Checkers::randomMove() {
	makeMove(availableMoveList_[rand() % availableMoveList_.size()]);
}


void Checkers::makeMove(Move move) {
	checkerboard_.executeMove(move);
	previousMoveList_.push_back(move);
	switchPlayers();
}


void Checkers::undo() {
	if (!previousMoveList_.empty()) {
		checkerboard_.reverseMove(previousMoveList_.back());
		previousMoveList_.pop_back();
		switchPlayers();
	}
}
