#pragma once

#include <string>



struct CheckersPlayer {
public:
	char player;
	int vertical_direction;
	bool is_user_controlled;

	
	bool has_piece(char piece) {
		return tolower(piece) == player;
	}
};
