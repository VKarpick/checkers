#include "checkers_sfml.h"



void CheckersSFML::play() {
	window_.create(sf::VideoMode(constants::window_size, constants::window_size), "Checkers");
	while (window_.isOpen()) {
		sf::Event event;
		while (window_.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window_.close();
			}

			window_.clear(sf::Color::Black);

			Checkers::play();
		}
	}
}


void CheckersSFML::render() {
	window_.draw(checkerboard_);
	window_.display();
}
