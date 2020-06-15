#include "start_screen.h"
#include <iostream>


StartScreen::StartScreen() {
	font_.loadFromFile("..\\checkers\\data\\arial.ttf");
}


StartScreen::StartScreenSelection StartScreen::show(sf::RenderWindow& window) {
	sf::Text select_text{ text_for_display("Select which color(s)\n   you intend to use") };
	select_text.setPosition((window.getSize().x - select_text.getLocalBounds().width) / 2, 80);

	float button_x{ select_text.getPosition().x };
	float button_width{ select_text.getLocalBounds().width };
	float button_height{ 42 };

	StartScreenItem play_button{ create_button(button_x, 315, button_height, button_width, StartScreen::StartScreenSelection::Play, false) };
	start_screen_items_.push_back(play_button);
	sf::Text play_text{ text_for_display("Play") };
	play_text.setPosition((window.getSize().x - play_text.getLocalBounds().width) / 2, play_button.rect.top + 2);

	StartScreenItem exit_button{ create_button(button_x, 390, button_height, button_width, StartScreen::StartScreenSelection::Exit, false) };
	start_screen_items_.push_back(exit_button);
	sf::Text exit_text{ text_for_display("Exit") };
	exit_text.setPosition(play_text.getPosition().x, exit_button.rect.top + 2);

	int square_size{ constants::checkerboard_square_size };
	StartScreenItem red_button{ create_button(128, 192, square_size, square_size, StartScreen::StartScreenSelection::Red, highlight_red) };
	CheckersPiece red_piece{ constants::pieces[0], red_button.rect.top / square_size, red_button.rect.left / square_size, false };
	start_screen_items_.push_back(red_button);

	StartScreenItem white_button{ create_button(320, 192, square_size, square_size, StartScreen::StartScreenSelection::White, highlight_white) };
	CheckersPiece white_piece{ constants::pieces[1], white_button.rect.top / square_size, white_button.rect.left / square_size, false };
	start_screen_items_.push_back(white_button);

	StartScreen::StartScreenSelection input;
	do {
		window.draw(select_text);
		window.draw(play_button.shape);
		window.draw(play_text);
		window.draw(exit_button.shape);
		window.draw(exit_text);

		window.draw(red_button.shape);
		window.draw(red_piece);

		window.draw(white_button.shape);
		window.draw(white_piece);

		window.display();

		input = get_input(window);

		if (input == StartScreenSelection::Red) { 
			highlight_red = !highlight_red;
			red_button.shape.setOutlineColor((highlight_red) ? sf::Color::Yellow : sf::Color::White);
		}
		if (input == StartScreenSelection::White) {
			highlight_white = !highlight_white;
			white_button.shape.setOutlineColor((highlight_white) ? sf::Color::Yellow : sf::Color::White);
		}
	} while (input != StartScreenSelection::Play && input != StartScreenSelection::Exit);
	
	return input;
}


StartScreen::StartScreenSelection StartScreen::get_input(sf::RenderWindow& window) {
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				return StartScreenSelection::Exit;
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				return handle_click(event.mouseButton.x, event.mouseButton.y);
			}
		}
	}
}


sf::Text StartScreen::text_for_display(std::string message) {
	sf::Text text(message, font_);
	text.setCharacterSize(32);
	text.setFillColor(sf::Color::White);
	
	return text;
}


StartScreen::StartScreenItem StartScreen::create_button(int left, int top, int height, int width, 
														StartScreenSelection action, bool is_selected) {
	StartScreenItem button;
	button.rect.left = left;
	button.rect.top = top;
	button.rect.height = height;
	button.rect.width = width;
	button.shape.setSize(sf::Vector2f(width, height));
	button.shape.setPosition(left, top);
	button.shape.setFillColor(sf::Color::Black);
	button.shape.setOutlineColor((is_selected) ? sf::Color::Yellow : sf::Color::White);
	button.shape.setOutlineThickness(1);
	button.action = action;
	
	return button;
}


StartScreen::StartScreenSelection StartScreen::handle_click(int x, int y) {
	std::list<StartScreen::StartScreenItem>::iterator it;

	for (it = start_screen_items_.begin(); it != start_screen_items_.end(); ++it) {
		sf::Rect<int> item_rect{ (*it).rect };

		if (x > item_rect.left && x < (item_rect.left + item_rect.width)
			&& y > item_rect.top && y < (item_rect.top + item_rect.height)) {
			return (*it).action;
		}
	}

	return StartScreenSelection::Nothing;
}
