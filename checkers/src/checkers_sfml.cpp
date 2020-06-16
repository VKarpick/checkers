#include "checkers_sfml.h"



void CheckersSFML::start() {
	window_.create(sf::VideoMode(constants::window_width, constants::window_height), "Checkers", sf::Style::Close);
	window_.clear(sf::Color::Black);
	window_.display();

	font_.loadFromFile("..\\checkers\\data\\arial.ttf");

	int top{ constants::window_height - constants::checkerboard_square_size + 1 };
	int height{ constants::checkerboard_square_size - 1 };
	int n_buttons{ 4 };
	int width{ constants::window_width / n_buttons - 1 };
	new_game_button_ = create_button(top, 1, height, width, ButtonSelection::NewGame);
	undo_button_ = create_button(top, width + 2, height, width, ButtonSelection::Undo);
	redo_button_ = create_button(top, (width + 1) * 2, height, width, ButtonSelection::Redo);
	players_button_ = create_button(top, (width + 1) * 3, height, width, ButtonSelection::Players);

	Button thinking_button;
	thinking_button.shape.setPosition(new_game_button_.shape.getPosition());
	thinking_button.shape.setSize(sf::Vector2f(constants::window_width, constants::checkerboard_square_size));
	thinking_button.shape.setOutlineColor(sf::Color::White);
	thinking_text_ = button_text(thinking_button, "Thinking ...");

	Checkers::start();
}


CheckersSFML::Button CheckersSFML::create_button(int top, int left, int height, int width, ButtonSelection action) {
	Button button;
	button.shape.setSize(sf::Vector2f(width, height));
	button.shape.setPosition(left, top);
	button.shape.setFillColor(sf::Color::Black);
	button.shape.setOutlineColor(constants::grey);
	button.shape.setOutlineThickness(1);
	button.action = action;

	return button;
}


void CheckersSFML::render_start_screen() {
	StartScreen start_screen;
	StartScreen::StartScreenSelection selection{ start_screen.show(window_) };

	if (selection == StartScreen::StartScreenSelection::Exit) {
		state_ = CheckersState::Exiting;
	}
	else {
		players_ = {
			CheckersPlayer{ constants::pieces[0], -1, start_screen.highlight_red },
			CheckersPlayer{ constants::pieces[1], 1, start_screen.highlight_white }
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
	new_game_button_.shape.setOutlineColor((previous_moves_.empty()) ? constants::grey : sf::Color::White);
	window_.draw(new_game_button_.shape);
	window_.draw(button_text(new_game_button_, "New\nGame"));
	
	if (!is_game_over) {
		int n_users{ (current_player_.is_user_controlled) ? 1 : 0 + (opponent_.is_user_controlled) ? 1 : 0 };
		undo_button_.shape.setOutlineColor((n_users >= previous_moves_.size()) ? constants::grey : sf::Color::White);
		window_.draw(undo_button_.shape);
		window_.draw(button_text(undo_button_, "Undo"));

		redo_button_.shape.setOutlineColor((redo_moves_.empty()) ? constants::grey : sf::Color::White);
		window_.draw(redo_button_.shape);
		window_.draw(button_text(redo_button_, "Redo"));
	}

	players_button_.shape.setOutlineColor(sf::Color::White);
	window_.draw(players_button_.shape);
	window_.draw(button_text(players_button_, "Change\nPlayers"));
}


void CheckersSFML::process_input() {
	while (window_.isOpen()) {
		sf::Event event;
		while (window_.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				state_ = CheckersState::Exiting;
			}
		}

		Checkers::process_input();
		return;
	}
}


std::string CheckersSFML::get_user_input() {
	return "r";
}
