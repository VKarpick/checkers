#include "checkerboard.h"



Checkerboard::Checkerboard() {
	reset();
}


Checkerboard::Checkerboard(std::vector<std::string> board) {
	board_ = board;
}


void Checkerboard::reset() {
	board_ = constants::kStartingBoard;
}


void Checkerboard::print() {
	for (auto row : board_) {
		std::cout << row << std::endl;
	}
}


char Checkerboard::getPiece(BoardPosition position) {
	return board_[position.row][position.column];
}


void Checkerboard::executeMove(Move move) {
	char pieceChar{ getPiece(move.startPosition) };

	// pieces moves from starting position to last landing position
	board_[move.startPosition.row][move.startPosition.column] = constants::kOpening;
	board_[move.landingPositions.back().row][move.landingPositions.back().column] = (move.isCrowning) ? toupper(pieceChar) : pieceChar;

	// all captured pieces are removed
	for (BoardPosition position : move.capturedPositions) {
		board_[position.row][position.column] = constants::kOpening;
	}
}


Checkerboard Checkerboard::padBoard(int padding) {
	std::vector<std::string> paddedBoard{};
	std::string columnPadding(padding, ' ');

	// pad rows before board
	for (int i = 0; i < padding; ++i) {
		paddedBoard.push_back(columnPadding + std::string(constants::kBoardSize, ' ') + columnPadding);
	}
	// pad each side of the row
	for (std::string row : board_) {
		paddedBoard.push_back(columnPadding + row + columnPadding);
	}
	// pad rows after board
	for (int i = 0; i < padding; ++i) {
		paddedBoard.push_back(columnPadding + std::string(constants::kBoardSize, ' ') + columnPadding);
	}

	return Checkerboard(paddedBoard);
}
