#include "checkers_piece.h"


CheckersPiece::CheckersPiece(char piece, int row, int column, bool is_king) :
	piece_(piece),
	row_(row),
	column_(column),
	is_king_(is_king)
{
	bottom_piece_ = piece_shape();
	top_piece_ = piece_shape();

	const float x{ float(column_ * constants::checkerboard_square_size + constants::square_padding) };
	const float y{ float(row_ * constants::checkerboard_square_size + constants::square_padding) };

	const float piece_spacing{ constants::piece_thickness + 1.f };
	bottom_piece_.setPosition(x, y + ((is_king_) ? piece_spacing : 0.f));
	top_piece_.setPosition(x, y - ((is_king_) ? piece_spacing : 0.f));
}


void CheckersPiece::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	target.draw(bottom_piece_);
	if (is_king_) {
		target.draw(top_piece_);
	}
}


sf::CircleShape CheckersPiece::piece_shape() {
	sf::CircleShape piece(constants::piece_size);
	piece.setFillColor((piece_ == constants::pieces[0]) ? constants::piece_colors[0] : constants::piece_colors[1]);
	piece.setOutlineThickness(constants::piece_thickness);
	piece.setOutlineColor(sf::Color::Black);
	return piece;
}
