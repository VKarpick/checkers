#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>



namespace constants {
	constexpr int checkerboard_square_size{ 64 };
	constexpr int window_size{ checkerboard_square_size * 8 };
	constexpr int checkerboard_square_thickness{ 4 };
	constexpr int piece_size((checkerboard_square_size - checkerboard_square_thickness * 5) / 2);
	constexpr int piece_thickness{ checkerboard_square_thickness / 2 };
	constexpr int square_padding{ constants::checkerboard_square_thickness * 2 + constants::piece_thickness };

	const sf::Color buff{ sf::Color(240, 220, 130) };
	const sf::Color green{ sf::Color(0, 150, 0) };
	const std::vector<sf::Color> checkerboard_colors{ buff, green };

	constexpr int step_distance{ 1 };    // how far a piece is allowed to step
	constexpr int jump_distance{ 2 };    // how far a piece is allowed to jump
	constexpr int capture_distance{ 1 };    // how far away a piece can be to be captured
	const std::vector<int> column_moves{ -1, 1 };    // all pieces are allowed to move left or right

	const std::string pieces{ "rw" };    // how the pieces are represented on the board
	const std::vector<sf::Color> piece_colors{ sf::Color::Red, sf::Color::White };
	constexpr char board_opening{ '-' };    // how available spots on the board are represented
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
