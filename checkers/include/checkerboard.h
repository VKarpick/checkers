/*-------------------------------------------------------------------------------------------------
 Simple checkerboard for a game of Checkers.
-------------------------------------------------------------------------------------------------*/


#pragma once

#include <algorithm>
#include <iostream>

#include "checkers_piece.h"
#include "checkers_structs.h"


class Checkerboard : public sf::Drawable {
public:
	Checkerboard();
	Checkerboard(std::vector<std::string> board);


	std::vector<BoardPosition> option_highlights;    // squares on the board available to be selected
	std::vector<BoardPosition> selected_highlights;    // squares on the board previously selected


	std::vector<std::string> get_board();


	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
	void reset();
	char get_piece(const BoardPosition board_position);    // returns ' ' if board_position is invalid
	std::vector<BoardPosition> get_player_positions(CheckersPlayer player);
	void execute_move(const Move move);
	void reverse_move(const Move move);
	bool is_king_row(const BoardPosition board_position);    // top and bottom rows of the board


private:
	std::vector<std::string> board_;
};


std::ostream& operator<< (std::ostream& out, Checkerboard& checkerboard);
