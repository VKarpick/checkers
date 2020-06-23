/*-------------------------------------------------------------------------------------------------
 Screen for selecting players to use in checkers_sfml.
-------------------------------------------------------------------------------------------------*/


#pragma once

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
	const int font_size_{ 32 };
	const float button_height{ 42.f };


	StartScreenSelection get_input(sf::RenderWindow& window);
	sf::Text text_for_display(const std::string message);
	StartScreenItem create_button(const float top, const float left, const float height, const float width, 
		const StartScreenSelection action, const bool is_selected);
	StartScreenSelection handle_click(const int x, const int y);
};
