#pragma once

#include <SFML/Graphics.hpp>

#include "checkers_piece.h"



class StartScreen {
public:
	StartScreen();


	bool highlight_red{ true };
	bool highlight_white{ false };
	enum class StartScreenSelection{ Red, White, Play, Exit, Nothing };
	
	
	struct StartScreenItem {
	public:
		sf::RectangleShape shape;
		StartScreenSelection action;
	};


	StartScreenSelection show(sf::RenderWindow& window);


private:
	std::vector<StartScreenItem> start_screen_items_;
	sf::Font font_;


	StartScreenSelection get_input(sf::RenderWindow& window);
	sf::Text text_for_display(std::string message);
	StartScreenItem create_button(int top, int left, int height, int width, StartScreenSelection action, bool is_selected);
	StartScreenSelection handle_click(int x, int y);
};
