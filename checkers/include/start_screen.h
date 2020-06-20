#pragma once

#include <SFML/Graphics.hpp>

#include "checkers_piece.h"



class StartScreen {
public:
	StartScreen();


	bool has_red_highlighted{ true };
	bool has_white_highlighted{ false };
	enum class StartScreenSelection{ Red, White, Play, Exit, Nothing };
	
	
	struct StartScreenItem {
	public:
		sf::RectangleShape shape;
		StartScreenSelection action{ StartScreenSelection::Nothing };
	};


	StartScreenSelection show(sf::RenderWindow& window);


private:
	std::vector<StartScreenItem> start_screen_items_;
	sf::Font font_;
	int font_size_{ 32 };


	StartScreenSelection get_input(sf::RenderWindow& window);
	sf::Text text_for_display(std::string message);
	StartScreenItem create_button(float top, float left, float height, float width, StartScreenSelection action, bool is_selected);
	StartScreenSelection handle_click(int x, int y);
};
