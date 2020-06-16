#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "checkers.h"
#include "start_screen.h"



class CheckersSFML : public Checkers{
public:
	CheckersSFML();


private:
	enum class ButtonSelection { NewGame, Undo, Redo, Players, Nothing };

	struct Button {
		sf::Color color;
		sf::RectangleShape shape;
		sf::Text text;
		ButtonSelection action;
	};

	sf::RenderWindow window_;
	sf::Font font_;
	sf::Text thinking_text_;
	std::vector<Button> buttons_;
	const int button_character_size{ 24 };


	StartScreen start_screen_;
	Button create_button(std::string text, int top, int left, int height, int width, ButtonSelection action);
	void render_start_screen() override;
	void render_end_screen() override;
	void render() override;
	static sf::Vector2f center_button_text(Button button, sf::Text text);
	sf::Text button_text(Button button, std::string message);
	void render_buttons(bool is_game_over);
	std::string get_user_input() override;
	ButtonSelection handle_button_click(int x);

};
