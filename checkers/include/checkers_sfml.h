/*-------------------------------------------------------------------------------------------------
 Version of Checkers utilizing SFML graphics.
-------------------------------------------------------------------------------------------------*/


#pragma once

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
		ButtonSelection action{ ButtonSelection::Nothing };
	};

	sf::RenderWindow window_;
	sf::Font font_;
	sf::Text thinking_text_;
	std::vector<Button> buttons_;
	const int button_character_size{ 24 };
	Move current_selected_move_{};


	StartScreen start_screen_;
	Button create_button(std::string text, float top, float left, float height, float width, ButtonSelection action);
	void render_start_screen() override;
	void render_end_screen() override;
	void update() override;

	// remove any highlights for cells that have been selected and highlight any piece that can be moved
	void starting_highlights();

	void render() override;
	static sf::Vector2f center_button_text(Button button, sf::Text text);
	sf::Text button_text(Button button, std::string message);
	void render_buttons(bool is_game_over);
	std::string get_user_input() override;
	ButtonSelection handle_button_click(int x);
	void update_highlights(BoardPosition square_clicked);
	bool is_option(BoardPosition square_clicked);
	void update_click_options(BoardPosition square_clicked);
	void reset_current_move();
	int legal_move_check();    // illegal moves represented by -1, legal moves by their index
};
