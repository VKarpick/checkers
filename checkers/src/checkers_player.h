#pragma once

#include <string>



struct CheckersPlayer {
public:
	char player_char;
	int vertical_direction;
	bool is_user_controlled;

	
	bool has_piece(char piece) {
		return tolower(piece) == player_char;
	}
};
