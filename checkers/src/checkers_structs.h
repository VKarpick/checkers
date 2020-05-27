#pragma once

#include <vector>



struct BoardPosition {
	int row;
	int column;


	bool operator==(const BoardPosition& boardPosition) const {
		return row == boardPosition.row && column == boardPosition.column;
	}
};



struct Piece {
	BoardPosition position;
	char pieceChar;
};



struct Move {
	BoardPosition startPosition;
	std::vector<BoardPosition> landingPositions;
	bool isCrowning;
	std::vector<Piece> capturedPieces{};


	bool operator==(const Move& move) const {
		return startPosition == move.startPosition && landingPositions == move.landingPositions;
	}
};
