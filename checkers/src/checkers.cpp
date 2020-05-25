#include "checkers.h"



//TODO remove this
void Checkers::printMoveBoards() { 
	update();
	for (Move move : availableMoveList_) {
		Checkerboard newBoard{ checkerboard_ };
		newBoard.executeMove(move);
		newBoard.print();
	}
}


void Checkers::play() {
	checkerboard_.print();
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
	Checkerboard paddedBoard{ checkerboard_.padBoard(constants::kJumpDistance) };

	for (int row = 0; row < constants::kBoardSize; ++row) {
		// pieces can only be in every other column
		for (int column = (row + 1) % 2; row < constants::kBoardSize; column += 2) {
			BoardPosition piecePosition{ row, column };
			char pieceChar{ checkerboard_.getPiece(piecePosition) };
			
			if (tolower(pieceChar) == currentPlayerChar_) {
				std::vector<int> rowMoves{ allowableRowMoves(pieceChar) };

				std::vector<Move> pieceJumps{ pieceMoves(paddedBoard, piecePosition, constants::kJumpDistance, rowMoves, true) };
				jumps.insert(jumps.end(), pieceJumps.begin(), pieceJumps.end());

				// jumps are forced so only need to look for steps if no jumps are available
				if (jumps.empty()) {
					std::vector<Move> pieceSteps{ pieceMoves(paddedBoard, piecePosition, constants::kStepDistance, rowMoves, false) };
					steps.insert(steps.end(), pieceSteps.begin(), pieceSteps.end());
				}
			}
		}
	}

	availableMoveList_ = (jumps.empty()) ? steps : jumps;
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


// assumes board has been properly padded
std::vector<Move> Checkers::pieceMoves(Checkerboard board, BoardPosition piecePosition, 
	int moveDistance, std::vector<int> rowMoves, bool canCapture) {

	std::vector<Move> moves;

	for (int rowMove : rowMoves) {
		for (int columnMove : constants::kColumnMoves) {

			BoardPosition movePosition{ piecePosition.row + rowMove * moveDistance, piecePosition.column + columnMove * moveDistance };
			// can only move to open squares
			bool moveAvailable{ board.getPiece(movePosition) == constants::kOpening };

			bool captureAvailable{ true };    // initialize to true to simplify if statement
			BoardPosition capturePosition;
			if (canCapture) {
				capturePosition.row = piecePosition.row + rowMove * constants::kCaptureDistance;
				capturePosition.column = piecePosition.column + columnMove * constants::kCaptureDistance;
				// can only capture if opponent's piece in capturePosition
				captureAvailable = tolower(board.getPiece(capturePosition)) == opponentChar_ ;
			}
			
			if (moveAvailable && captureAvailable) {
				Move currentMove{ piecePosition, { movePosition }, isCrowning(board.getPiece(piecePosition), movePosition) };

				if (canCapture) {
					currentMove.capturedPositions = { capturePosition };

					// for jumps, have to continue jumping until no more jumps available
					Checkerboard afterMoveBoard = board;
					afterMoveBoard.executeMove(currentMove);

					// use recursion to continue jump
					std::vector<Move> continueJump{ pieceMoves(afterMoveBoard, movePosition, moveDistance, rowMoves, canCapture) };
					
					for (Move jump : continueJump) {
						std::vector<BoardPosition> landings{ currentMove.landingPositions };
						landings.insert(landings.end(), jump.landingPositions.begin(), jump.landingPositions.end());

						std::vector<BoardPosition> captures{ currentMove.capturedPositions };
						captures.insert(captures.end(), jump.capturedPositions.begin(), jump.capturedPositions.end());

						Move newMove{ currentMove.startPosition, landings, jump.isCrowning, captures };
						moves.push_back(newMove);
					}
				}
				else {
					// steps end with the step
					moves.push_back(currentMove);
				}
			}
		}
	}

	return moves;
}


// pieces are crowned if they aren't already kings and can reach the top/bottom of the unpadded board
// assumes board has been properly padded
bool Checkers::isCrowning(char pieceChar, BoardPosition boardPosition) {
	bool isKing{ pieceChar == toupper(pieceChar) };
	bool isKingRow{ boardPosition.row == constants::kJumpDistance 
					|| boardPosition.row == constants::kBoardSize - constants::kJumpDistance };

	return !isKing && isKingRow;
}
