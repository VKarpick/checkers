#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "checkers_piece.h"
#include "checkers_structs.h"
#include "constants.h"



class Checkerboard : public sf::Drawable {
public:
	Checkerboard();
	Checkerboard(std::vector<std::string> board);


	std::vector<std::string> get_board();


	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
	void reset();
	char get_piece(BoardPosition board_position);
	std::vector<BoardPosition> get_player_positions(CheckersPlayer player);
	void execute_move(Move move);
	void reverse_move(Move move);
	bool is_king_row(BoardPosition board_position);


private:
	std::vector<std::string> board_;
};



std::ostream& operator<< (std::ostream& out, Checkerboard& checkerboard);
