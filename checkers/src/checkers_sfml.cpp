#include "checkers_sfml.h"



CheckersSFML::CheckersSFML() {
	window_.create(sf::VideoMode(constants::window_width, constants::window_height), "Checkers", sf::Style::Close);

	font_.loadFromFile("..\\checkers\\data\\arial.ttf");

	int top{ constants::window_height - constants::checkerboard_square_size + 1 };
	int height{ constants::checkerboard_square_size - 1 };
	int n_buttons{ 4 };
	int width{ constants::window_width / n_buttons - 1 };
	buttons_.push_back(create_button("New\nGame", top, 1, height, width, ButtonSelection::NewGame));
	buttons_.push_back(create_button("Undo", top, width + 2, height, width, ButtonSelection::Undo));
	buttons_.push_back(create_button("Redo", top, (width + 1) * 2, height, width, ButtonSelection::Redo));
	buttons_.push_back(create_button("Change\nPlayers", top, (width + 1) * 3, height, width, ButtonSelection::Players));

	Button thinking_button;
	thinking_button.shape.setPosition(buttons_[0].shape.getPosition());
	thinking_button.shape.setSize(sf::Vector2f(constants::window_width, constants::checkerboard_square_size));
	thinking_button.shape.setOutlineColor(sf::Color::White);
	thinking_text_ = button_text(thinking_button, "Thinking ...");
}


CheckersSFML::Button CheckersSFML::create_button(std::string text, int top, int left, int height, int width, ButtonSelection action) {
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
			CheckersPlayer{ constants::pieces[0], -1, start_screen_.highlight_red },
			CheckersPlayer{ constants::pieces[1], 1, start_screen_.highlight_white }
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
		window_.display();

		std::string input{ get_user_input() };
		if (input == "n" || input == "c") {
			process_input(input);
			return;
		}
	}
}


void CheckersSFML::render() {
	window_.clear(sf::Color::Black);
	window_.draw(checkerboard_);
	if (current_player_.is_user_controlled) {
		render_buttons(false);
	}
	else {
		window_.draw(thinking_text_);
	}
	window_.display();
}


sf::Vector2f CheckersSFML::center_button_text(CheckersSFML::Button button, sf::Text text) {
	return sf::Vector2f(button.shape.getPosition().x + (button.shape.getSize().x - double(text.getLocalBounds().width)) / 2.2,  
						button.shape.getPosition().y + (button.shape.getSize().y - double(text.getLocalBounds().height)) / 3);
}


sf::Text CheckersSFML::button_text(Button button, std::string message) {
	sf::Text text(message, font_);
	text.setCharacterSize(button_character_size);
	text.setFillColor(button.shape.getOutlineColor());
	text.setPosition(center_button_text(button, text));

	return text;
}


void CheckersSFML::render_buttons(bool is_game_over) {
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

	for (int i = 0; i < grey_check.size(); ++i) {
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

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.y > constants::window_height - constants::checkerboard_square_size) {
					ButtonSelection selection{ handle_button_click(event.mouseButton.x) };
					if (selection == ButtonSelection::NewGame) { return "n"; }
					else if (selection == ButtonSelection::Undo) { return "z"; }
					else if (selection == ButtonSelection::Redo) { return "y"; }
					else if (selection == ButtonSelection::Players) { return "c"; }
					else { return ""; }
				}
				else {
					return "r";
				}
			}
		}
	}
}


CheckersSFML::ButtonSelection CheckersSFML::handle_button_click(int x) {
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
