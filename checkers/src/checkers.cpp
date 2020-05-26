#include "checkers.h"



//TODO remove this
void Checkers::printMoveBoards() { 
	update();
	for (Move move : availableMoveList_) {
		Checkerboard newBoard{ checkerboard_ };
		newBoard.executeMove(move);
		std::cout << newBoard << std::endl;
	}
}


//TODO remove this
void Checkers::boardTry() {
	currentPlayerChar_ = 'w';
	opponentChar_ = 'r';
	checkerboard_ = { Checkerboard({
		" w w - w",
		"w w w w ",
		" w - w r",
		"- w w - ",
		" - - - -",
		"r w w r ",
		" - - w r",
		"r r r - ",
	} ) };
}


void Checkers::play() {
	reset();

	while (!availableMoveList_.empty()) {
		update();
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

		std::string moveIndex;
		std::cin >> moveIndex;
		
	}
}


void Checkers::reset() {
	checkerboard_.reset();
	currentPlayerChar_ = constants::kPlayerChars[0];
	opponentChar_ = constants::kPlayerChars[1];
	update();
}


void Checkers::update() {
	std::vector<Move> jumps{};
	std::vector<Move> steps{};

	for (int row = 0; row < constants::kBoardSize; ++row) {
		// pieces can only be in every other column
		for (int column = (row + 1) % 2; column < constants::kBoardSize; column += 2) {
			BoardPosition piecePosition{ row, column };
			char pieceChar{ checkerboard_.getPiece(piecePosition) };
			
			if (tolower(pieceChar) == currentPlayerChar_) {
				std::vector<int> rowMoves{ allowableRowMoves(pieceChar) };

				std::vector<Move> pieceJumps{ pieceMoves(checkerboard_, piecePosition, constants::kJumpDistance, rowMoves, true) };
				jumps.insert(jumps.end(), pieceJumps.begin(), pieceJumps.end());

				// jumps are forced so only need to look for steps if no jumps are available
				if (jumps.empty()) {
					std::vector<Move> pieceSteps{ pieceMoves(checkerboard_, piecePosition, constants::kStepDistance, rowMoves, false) };
					steps.insert(steps.end(), pieceSteps.begin(), pieceSteps.end());
				}
			}
		}
	}

	availableMoveList_ = (jumps.empty()) ? steps : jumps;
}


void Checkers::executeInputMove(int moveIndex) {
	update();
	checkerboard_.executeMove(availableMoveList_[moveIndex]);
	std::swap(currentPlayerChar_, opponentChar_);
}


std::vector<int> Checkers::allowableRowMoves(char pieceChar) {
	if (pieceChar == constants::kPlayerChars[0]) {
		return { -1 };    // first player can only move up
	}
	else if (pieceChar == constants::kPlayerChars[1]) {
		return { 1 };    // second player can only move down
	}
	else {
		return { -1, 1 };    // kings can move up or down
	}
}


std::vector<Move> Checkers::pieceMoves(Checkerboard board, BoardPosition piecePosition, 
	int moveDistance, std::vector<int> rowMoves, bool canCapture) {

	std::vector<Move> moves;

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
				captureAvailable = tolower(board.getPiece(capturePosition)) == opponentChar_ ;
			}

			if (moveAvailable && captureAvailable) {
				Move currentMove{ piecePosition, { movePosition }, isCrowningMove(board.getPiece(piecePosition), movePosition) };

				if (canCapture) {
					currentMove.capturedPositions = { capturePosition };

					// for jumps, have to continue jumping until no more jumps available
					Checkerboard afterMoveBoard = board;
					afterMoveBoard.executeMove(currentMove);

					// use recursion to continue jump
					std::vector<Move> continueJump{ pieceMoves(afterMoveBoard, movePosition, moveDistance, rowMoves, canCapture) };
					
					for (Move jump : continueJump) {
						currentMove.landingPositions.insert(
							currentMove.landingPositions.end(),
							jump.landingPositions.begin(),
							jump.landingPositions.end());

						currentMove.capturedPositions.insert(
							currentMove.capturedPositions.end(),
							jump.capturedPositions.begin(),
							jump.capturedPositions.end());

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
