#pragma once

#include <SFML/Graphics.hpp>

#include "checkers.h"



class CheckersSFML : public Checkers{
public:
	void play();
	void render();


private:
	sf::RenderWindow window_;
};
