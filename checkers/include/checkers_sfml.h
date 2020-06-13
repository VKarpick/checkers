#pragma once

#include <SFML/Graphics.hpp>

#include "checkers.h"



class CheckersSFML : public Checkers{
public:
	CheckersSFML();


private:
	sf::RenderWindow window_;


	void render_board();
	void render_options();
};
