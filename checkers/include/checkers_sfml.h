#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "checkers.h"
#include "start_screen.h"



class CheckersSFML : public Checkers{
public:
	void start() override;


private:
	enum class ButtonSelection { NewGame, Undo, Redo, Players };

	struct Button {
		sf::RectangleShape shape;
		ButtonSelection action;
	};

	sf::RenderWindow window_;
	sf::Font font_;
	sf::Text thinking_text_;
	Button new_game_button_;
	Button undo_button_;
	Button redo_button_;
	Button players_button_;
	const int button_character_size{ 24 };


	Button create_button(int top, int left, int height, int width, ButtonSelection action);
	void render_start_screen() override;
	void render_end_screen() override;
	void render() override;
	static sf::Vector2f center_button_text(Button button, sf::Text text);
	sf::Text button_text(Button button, std::string message);
	void render_buttons(bool is_game_over);
	void process_input() override;
	std::string get_user_input() override;
};
