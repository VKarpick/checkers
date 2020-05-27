#pragma once

#include <vector>

#include "checkerboard.h"



struct BoardPosition {
	int row;
	int column;
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
};
