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


	bool operator==(const CheckersPlayer& checkers_player) const {
		return player == checkers_player.player 
			&& vertical_direction == checkers_player.vertical_direction 
			&& is_user_controlled == checkers_player.is_user_controlled;
	}
};
