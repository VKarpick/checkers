#pragma once

#include <string>
#include <vector>



namespace constants {
	//const std::string kPlayerChars{ "rw" };    // players are red and white, represented by first letter
	constexpr char board_opening{ '-' };    // how available spots on the board are represented
	constexpr int step_distance{ 1 };    // how far a piece is allowed to step
	constexpr int jump_distance{ 2 };    // how far a piece is allowed to jump
	constexpr int capture_distance{ 1 };    // how far away a piece can be to be captured
	const std::vector<int> column_moves{ -1, 1 };    // all pieces are allowed to move left or right
	const std::vector<std::string> starting_board{
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
