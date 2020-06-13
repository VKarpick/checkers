#pragma once

#include <string>
#include <vector>



namespace constants {
	constexpr char board_opening{ '-' };    // how available spots on the board are represented
	constexpr int step_distance{ 1 };    // how far a piece is allowed to step
	constexpr int jump_distance{ 2 };    // how far a piece is allowed to jump
	constexpr int capture_distance{ 1 };    // how far away a piece can be to be captured
	const std::vector<int> column_moves{ -1, 1 };    // all pieces are allowed to move left or right
	//TODO remove magic variables
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
	const int square_size{ 64 };
}
