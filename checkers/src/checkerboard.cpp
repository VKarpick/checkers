#include "checkerboard.h"



Checkerboard::Checkerboard() {
	reset();
}


Checkerboard::Checkerboard(std::vector<std::string> board) :
	board_(board)
{}


std::vector<std::string> Checkerboard::getBoard() {
	return board_;
}


void Checkerboard::reset() {
	board_ = constants::kStartingBoard;
}


char Checkerboard::getPiece(BoardPosition position) {
	bool legalRow{ position.row > -1 && position.row < constants::kBoardSize };
	bool legalColumn{ position.column > -1 && position.column < constants::kBoardSize };
	if (legalRow && legalColumn) {
		return board_[position.row][position.column];
	}
	else {
		return ' ';
	}
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



std::ostream& operator<< (std::ostream& out, Checkerboard& checkerboard) {
	std::vector<std::string> board{ checkerboard.getBoard() };
	for (std::string row : checkerboard.getBoard()) {
		out << row << std::endl;
	}
	return out;
}
