#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "checkers.h"



class CheckersSFML : public Checkers{
public:
	void start();
	void render();


private:
	sf::RenderWindow window_;
};
