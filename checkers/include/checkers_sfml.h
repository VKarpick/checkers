#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "checkers.h"
#include "start_screen.h"



class CheckersSFML : public Checkers{
public:
	void start() override;


private:
	sf::RenderWindow window_;


	void render_start_screen() override;
	void render_end_screen() override;
	void render() override;
};
