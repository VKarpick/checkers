#include "checkers_sfml.h"



void CheckersSFML::start() {
	window_.create(sf::VideoMode(constants::window_size, constants::window_size), "Checkers");
	window_.clear(sf::Color::Black);
	window_.display();

	Checkers::start();
}


void CheckersSFML::render_start_screen() {
	StartScreen start_screen;
	StartScreen::StartScreenSelection selection{ start_screen.show(window_) };

	if (selection == StartScreen::StartScreenSelection::Exit) {
		window_.close();
	}
	else {
		players_ = {
			CheckersPlayer{ constants::pieces[0], -1, start_screen.highlight_red },
			CheckersPlayer{ constants::pieces[1], 1, start_screen.highlight_white }
		};

		reset();
		state_ = CheckersState::Playing;
	}
}


void CheckersSFML::render_end_screen() {
	
}


void CheckersSFML::render() {
	window_.draw(checkerboard_);
	window_.display();
}
