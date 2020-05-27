#pragma once

#include <vector>



struct BoardPosition {
	int row{ -1 };
	int column{ -1 };
};



struct Move {
	BoardPosition startPosition;
	std::vector<BoardPosition> landingPositions;
	bool isCrowning{ false };
	std::vector<BoardPosition> capturedPositions{};
};
