/*-------------------------------------------------------------------------------------------------
 Simple class for Checkers pieces.
-------------------------------------------------------------------------------------------------*/


#pragma once

#include <SFML/Graphics.hpp>

#include "constants.h"


class CheckersPiece : public sf::Drawable {
public:
	CheckersPiece(char piece, int row, int column, bool is_king);


	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;


private:
	char piece_;
	int row_;
	int column_;
	bool is_king_;
	sf::CircleShape bottom_piece_;    // regular pieces only have a bottom piece
	sf::CircleShape top_piece_;    // kings are displayed as two pieces, one on top of the other


	sf::CircleShape piece_shape();
};
