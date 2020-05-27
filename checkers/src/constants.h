#pragma once

#include <string>
#include <vector>



namespace constants {
	//const std::string kPlayerChars{ "rw" };    // players are red and white, represented by first letter
	constexpr char kOpening{ '-' };    // how available spots on the board are represented
	constexpr int kBoardSize{ 8 };    // checkerboards are 8x8
	constexpr int kStepDistance{ 1 };    // how far a piece is allowed to step
	constexpr int kJumpDistance{ 2 };    // how far a piece is allowed to jump
	constexpr int kCaptureDistance{ 1 };    // how far away a piece can be to be captured
	const std::vector<int> kColumnMoves{ -1, 1 };    // all pieces are allowed to move left or right
	const std::vector<std::string> kStartingBoard{
		" w w w w",
		"w w w w ",
		" w w w w",
		"- - - - ",
		" - - - -",
		"r r r r ",
		" r r r r",
		"r r r r ",
	};
}
