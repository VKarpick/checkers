#include "start_screen.h"



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
	play_text.setPosition((window.getSize().x - play_text.getLocalBounds().width) / 2, play_button.shape.getPosition().y + 2);

	StartScreenItem exit_button{ create_button(button_x, 390, button_height, button_width, StartScreen::StartScreenSelection::Exit, false) };
	start_screen_items_.push_back(exit_button);
	sf::Text exit_text{ text_for_display("Exit") };
	exit_text.setPosition(play_text.getPosition().x, exit_button.shape.getPosition().y + 2);

	int square_size{ constants::checkerboard_square_size };
	StartScreenItem red_button{ create_button(128, 192, square_size, square_size, StartScreen::StartScreenSelection::Red, highlight_red) };
	int red_x{ int(red_button.shape.getPosition().x) / square_size };
	int red_y{ int(red_button.shape.getPosition().y) / square_size };
	CheckersPiece red_piece{ constants::pieces[0], red_y, red_x, false };
	start_screen_items_.push_back(red_button);

	StartScreenItem white_button{ create_button(320, 192, square_size, square_size, StartScreen::StartScreenSelection::White, highlight_white) };
	int white_x{ static_cast<int>(white_button.shape.getPosition().x) / square_size };
	int white_y{ static_cast<int>(white_button.shape.getPosition().y) / square_size };
	CheckersPiece white_piece{ constants::pieces[1], white_y, white_x, false };
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
	button.shape.setSize(sf::Vector2f(width, height));
	button.shape.setPosition(left, top);
	button.shape.setFillColor(sf::Color::Black);
	button.shape.setOutlineColor((is_selected) ? sf::Color::Yellow : sf::Color::White);
	button.shape.setOutlineThickness(1);
	button.action = action;

	return button;
}


StartScreen::StartScreenSelection StartScreen::handle_click(int x, int y) {
	std::vector<StartScreen::StartScreenItem>::iterator it;
	for (it = start_screen_items_.begin(); it != start_screen_items_.end(); ++it) {
		sf::RectangleShape item_rectangle{ (*it).shape };

		if (x > item_rectangle.getPosition().x && x < item_rectangle.getPosition().x + item_rectangle.getSize().x
			&& y > item_rectangle.getPosition().y&& y < item_rectangle.getPosition().y + item_rectangle.getSize().y) {
			
			return (*it).action;
		}
	}

	return StartScreenSelection::Nothing;
}
