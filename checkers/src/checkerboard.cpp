#include "checkerboard.h"



Checkerboard::Checkerboard() {
	reset();
}


Checkerboard::Checkerboard(std::vector<std::string> board) :
	board_(board)
{}


std::vector<std::string> Checkerboard::get_board() {
	return board_;
}


void Checkerboard::draw(sf::RenderTarget& target, sf::RenderStates state) const {
	float inner_square_size{ float(constants::checkerboard_square_size) * 7.f / 8.f };
	for (size_t row = 0; row < board_.size(); ++row) {
		for (size_t column = 0; column < board_[row].size(); ++column) {
			// checkerboard squares
			sf::RectangleShape square(sf::Vector2f(inner_square_size, inner_square_size));
			square.setFillColor(((row + column) % 2 == 0) ? constants::checkerboard_colors[0] : constants::checkerboard_colors[1]);
			square.setPosition(sf::Vector2f(float(column * constants::checkerboard_square_size + constants::checkerboard_square_thickness), 
											float(row * constants::checkerboard_square_size + constants::checkerboard_square_thickness)));
			square.setOutlineThickness(constants::checkerboard_square_thickness);

			BoardPosition position{ int(row), int(column) };
			if (std::find(selected_highlights.begin(), selected_highlights.end(), position) != selected_highlights.end()) {
				square.setOutlineColor(sf::Color::Blue);
			}
			else {
				bool is_option{ std::find(option_highlights.begin(), option_highlights.end(), position) != option_highlights.end() };
				square.setOutlineColor((is_option) ? sf::Color::Yellow : square.getFillColor());
			}

			target.draw(square, state);

			// pieces
			char piece{ board_[row][column] };
			piece = tolower(piece);
			if (piece == constants::pieces[0] || piece == constants::pieces[1]) {
				CheckersPiece checkers_piece(piece, row, column, piece != board_[row][column]);
				target.draw(checkers_piece);
			}
		}
	}
}


void Checkerboard::reset() {
	board_ = constants::starting_board;
}


char Checkerboard::get_piece(BoardPosition position) {
	bool legal_row{ position.row > -1 && position.row < int(board_.size()) };
	bool legal_column{ position.column > -1 && position.column < int(board_.size()) };
	if (legal_row && legal_column) {
		return board_[position.row][position.column];
	}
	else {
		return ' ';
	}
}


std::vector<BoardPosition> Checkerboard::get_player_positions(CheckersPlayer player) {
	std::vector<BoardPosition> player_positions;

	for (size_t row = 0; row < board_.size(); ++row) {
		// pieces can only be in every other column
		for (size_t column = (row + 1) % 2; column < board_[row].size(); column += 2) {
			BoardPosition piece_position{ int(row), int(column) };

			if (player.has_piece(board_[row][column])) {
				player_positions.push_back(piece_position);
			}
		}
	}

	return player_positions;
}


void Checkerboard::execute_move(Move move) {
	char piece{ get_piece(move.start_position) };

	// piece moves from starting position to last landing position
	board_[move.start_position.row][move.start_position.column] = constants::board_opening;
	board_[move.landing_positions.back().row][move.landing_positions.back().column] = (move.is_crowning) ? toupper(piece) : piece;

	// all captured pieces are removed
	for (Piece captured_piece : move.captured_pieces) {
		board_[captured_piece.position.row][captured_piece.position.column] = constants::board_opening;
	}
}


void Checkerboard::reverse_move(Move move) {
	BoardPosition end_position{ move.landing_positions.back() };
	char piece{ get_piece(end_position) };
	
	if (move.is_crowning) piece = tolower(piece);

	// move piece back to its starting position
	board_[move.start_position.row][move.start_position.column] = piece;
	board_[end_position.row][end_position.column] = constants::board_opening;

	// add each captured piece back
	for (Piece captured_piece : move.captured_pieces) {
		board_[captured_piece.position.row][captured_piece.position.column] = captured_piece.piece;
	}
}


bool Checkerboard::is_king_row(BoardPosition board_position) {
	return board_position.row == 0 || board_position.row == board_.size() - 1;
}



std::ostream& operator<< (std::ostream& out, Checkerboard& checkerboard) {
	std::vector<std::string> board{ checkerboard.get_board() };
	for (std::string row : checkerboard.get_board()) {
		out << row << std::endl;
	}
	return out;
}
