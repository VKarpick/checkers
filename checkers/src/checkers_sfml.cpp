#include "checkers_sfml.h"


CheckersSFML::CheckersSFML() {
	window_.create(sf::VideoMode(constants::window_width, constants::window_height), "Checkers", sf::Style::Close);

	font_.loadFromFile("..\\checkers\\data\\arial.ttf");

	const float top{ constants::window_height - constants::checkerboard_square_size + 1.f };
	const float height{ constants::checkerboard_square_size - 1.f };
	const float n_buttons{ 4.f };
	const float width{ constants::window_width / n_buttons - 1.f };
	buttons_.push_back(create_button("New\nGame", top, 1.f, height, width, ButtonSelection::NewGame));
	buttons_.push_back(create_button("Undo", top, width + 2.f, height, width, ButtonSelection::Undo));
	buttons_.push_back(create_button("Redo", top, (width + 1.f) * 2.f, height, width, ButtonSelection::Redo));
	buttons_.push_back(create_button("Change\nPlayers", top, (width + 1.f) * 3.f, height, width, ButtonSelection::Players));

	Button thinking_button;
	thinking_button.shape.setPosition(buttons_[0].shape.getPosition());
	thinking_button.shape.setSize(sf::Vector2f(constants::window_width, constants::checkerboard_square_size));
	thinking_button.shape.setOutlineColor(sf::Color::White);
	thinking_text_ = button_text(thinking_button, "Thinking ...");
}


CheckersSFML::Button CheckersSFML::create_button(const std::string text, const float top, const float left, 
	const float height, const float width, const ButtonSelection action) {

	Button button;
	button.color = constants::grey;
	button.shape.setSize(sf::Vector2f(width, height));
	button.shape.setPosition(left, top);
	button.shape.setFillColor(sf::Color::Black);
	button.shape.setOutlineThickness(1);
	button.text = button_text(button, text);
	button.action = action;

	return button;
}


void CheckersSFML::render_start_screen() {
	StartScreen::StartScreenSelection selection{ start_screen_.show(window_) };

	if (selection == StartScreen::StartScreenSelection::Exit) {
		state_ = CheckersState::Exiting;
	}
	else {
		players_ = {
			CheckersPlayer{ constants::pieces[0], constants::player_directions[0], start_screen_.has_red_highlighted },
			CheckersPlayer{ constants::pieces[1], constants::player_directions[1], start_screen_.has_white_highlighted }
		};

		new_game();
	}
}


void CheckersSFML::render_end_screen() {
	while (window_.isOpen()) {
		sf::Event event;
		while (window_.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				state_ = CheckersState::Exiting;
				return;
			}
		}

		window_.clear(sf::Color::Black);
		window_.draw(checkerboard_);
		render_buttons(true);

		// create temporary button not used as a button to utilize other methods in displaying text
		Button win_button;
		win_button.shape.setPosition(0, constants::checkerboard_square_size * 3);
		win_button.shape.setSize(sf::Vector2f(constants::window_width, constants::checkerboard_square_size));
		win_button.shape.setOutlineColor(sf::Color::Black);
		sf::Text win_text{ button_text(win_button, won_lost_statement()) };
		window_.draw(win_text);
		window_.display();

		std::string input{ get_user_input() };
		if (input == "n" || input == "c") {    // n maps to new game, c maps to change players
			process_input(input);
			return;
		}
		else if (state_ == CheckersState::Exiting) {
			return;
		}
	}
}


void CheckersSFML::update() {
	Checkers::update();
	reset_current_move();
	starting_highlights();
}


void CheckersSFML::starting_highlights() {
	checkerboard_.selected_highlights.clear();
	checkerboard_.option_highlights.clear();
	std::vector<Move>::iterator it;
	for (it = available_moves_.begin(); it != available_moves_.end(); ++it) {
		checkerboard_.option_highlights.push_back((*it).start_position);
	}
}


void CheckersSFML::render() {
	window_.clear(sf::Color::Black);

	if (current_player_.is_user_controlled) {
		// if player is user controlled, draw buttons
		render_buttons(false);
	}
	else {
		// if player is ai controlled, let user know the AI is computing its move
		window_.draw(thinking_text_);
	}

	window_.draw(checkerboard_);
	window_.display();
}


sf::Vector2f CheckersSFML::center_button_text(const CheckersSFML::Button button, const sf::Text text) {
	sf::RectangleShape shape{ button.shape };
	return sf::Vector2f(float(shape.getPosition().x + (shape.getSize().x - double(text.getLocalBounds().width)) / 2.2), 
						float(shape.getPosition().y + (shape.getSize().y - double(text.getLocalBounds().height)) / 3.0));
}


sf::Text CheckersSFML::button_text(const Button button, const std::string message) {
	sf::Text text(message, font_);
	text.setCharacterSize(button_character_size);
	text.setFillColor(button.shape.getOutlineColor());
	text.setPosition(center_button_text(button, text));

	return text;
}


void CheckersSFML::render_buttons(const bool is_game_over) {
	std::vector<bool> grey_check{
		// new game
		previous_moves_.empty(),
		// undo
		previous_moves_.empty() || (previous_moves_.size() == 1 && !opponent_.is_user_controlled) || is_game_over,
		// redo
		redo_moves_.empty() || is_game_over,
		// change players
		false,
	};

	for (size_t i = 0; i < grey_check.size(); ++i) {
		sf::Color button_color{ (grey_check[i]) ? constants::grey : sf::Color::White };
		buttons_[i].color = button_color;
		buttons_[i].shape.setOutlineColor(button_color);
		buttons_[i].text.setFillColor(button_color);
		window_.draw(buttons_[i].shape);
		window_.draw(buttons_[i].text);
	}
}


std::string CheckersSFML::get_user_input() {
	while (window_.isOpen()) {
		sf::Event event;
		while (window_.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				state_ = CheckersState::Exiting;
				return "";
			}

			else if (event.type == sf::Event::MouseButtonPressed) {

				// buttons
				if (event.mouseButton.y > constants::window_height - constants::checkerboard_square_size) {
					ButtonSelection selection{ handle_button_click(event.mouseButton.x) };
					if (selection == ButtonSelection::NewGame) { return "n"; }
					else if (selection == ButtonSelection::Undo) { return "z"; }
					else if (selection == ButtonSelection::Redo) { return "y"; }
					else if (selection == ButtonSelection::Players) { return "c"; }
					else { return ""; }
				}

				// checkerboard squares
				else {
					if (state_ != CheckersState::EndScreen) {
						BoardPosition square_clicked{ event.mouseButton.y / constants::checkerboard_square_size,
													  event.mouseButton.x / constants::checkerboard_square_size };
						update_highlights(square_clicked);
						const int move{ legal_move_check() };
						if (move != -1) { return std::to_string(move); }
						render();
					}
				}
			}

			else if (event.type == sf::Event::KeyPressed) {

				// holding Ctrl and other pressing keys
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) { return "r"; }
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { return "q"; }
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) { return "z"; }
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) { return "y"; }
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) { return "c"; }
				}
			}
		}
	}

	return "";
}


CheckersSFML::ButtonSelection CheckersSFML::handle_button_click(const int x) {
	std::vector<Button>::iterator it;
	for (it = buttons_.begin(); it != buttons_.end(); ++it) {
		sf::RectangleShape button_rectangle{ (*it).shape };

		if (x > button_rectangle.getPosition().x && x < button_rectangle.getPosition().x + button_rectangle.getSize().x) {
			if (button_rectangle.getOutlineColor() == sf::Color::White) {
				return (*it).action;
			}
		}
	}
	return ButtonSelection::Nothing;
}


void CheckersSFML::update_highlights(const BoardPosition square_clicked) {
	if (is_option(square_clicked)) {
		update_click_options(square_clicked);
	}
	else{
		std::vector<BoardPosition>::iterator it{ std::find(checkerboard_.selected_highlights.begin(),
			checkerboard_.selected_highlights.end(), square_clicked) };

		// an intermediate of the current move was selected
		if (it != checkerboard_.selected_highlights.end()) {
			const int n_positions{ std::distance(checkerboard_.selected_highlights.begin(), it) };
			checkerboard_.selected_highlights.resize(n_positions);

			if (n_positions == 0) {
				reset_current_move();
			}
			else {
				current_selected_move_.landing_positions.resize(n_positions - 1);
			}
			
			update_click_options(square_clicked);
		}
		else {
			starting_highlights();
			reset_current_move();

			// if square clicked is one of the other possible starting positions, select it
			if (is_option(square_clicked)) {
				update_click_options(square_clicked);
			}
		}
	}
}


bool CheckersSFML::is_option(const BoardPosition square_clicked) {
	return std::find(checkerboard_.option_highlights.begin(),
		checkerboard_.option_highlights.end(), square_clicked) != checkerboard_.option_highlights.end();
}


void CheckersSFML::update_click_options(const BoardPosition square_clicked) {
	checkerboard_.option_highlights.clear();
	checkerboard_.selected_highlights.push_back(square_clicked);

	if (current_selected_move_.start_position.row == -1) {
		current_selected_move_.start_position = square_clicked;
	}
	else {
		current_selected_move_.landing_positions.push_back(square_clicked);
	}
}


void CheckersSFML::reset_current_move() {
	current_selected_move_.start_position = BoardPosition{ -1, -1 };    // -1, -1 used to represent that nothing is currently selected
	current_selected_move_.landing_positions.clear();
}


int CheckersSFML::legal_move_check() {
	for (size_t i = 0; i < available_moves_.size(); ++i) {
		std::vector<BoardPosition> available_landings{ available_moves_[i].landing_positions };
		available_landings.resize(current_selected_move_.landing_positions.size());
		const Move sub_move{ available_moves_[i].start_position, available_landings };

		if (available_moves_[i] == current_selected_move_) {
			return i;
		}
		else if (current_selected_move_ == sub_move) {
			checkerboard_.option_highlights.push_back(
				available_moves_[i].landing_positions[current_selected_move_.landing_positions.size()]);
		}
	}
	return -1;
}
