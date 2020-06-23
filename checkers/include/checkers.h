/*-------------------------------------------------------------------------------------------------
 Class for playing Checkers by printing to console.
-------------------------------------------------------------------------------------------------*/


#pragma once

#include <map>

#include "checkerboard.h"
#include "ai/checkers_ai.h"


class Checkers {
public:
	Checkers();


	std::vector<Move> get_available_moves();


	void start();


protected:
	enum class CheckersState { StartScreen, Playing, EndScreen, Exiting };
	CheckersState state_{ CheckersState::StartScreen };
	Checkerboard checkerboard_;
	CheckersAI checkers_ai_;
	std::vector<CheckersPlayer> players_;
	CheckersPlayer current_player_{ CheckersPlayer() };
	CheckersPlayer opponent_{ CheckersPlayer() };
	std::vector<Move> available_moves_{};
	std::vector<Move> previous_moves_{};
	std::vector<Move> redo_moves_{};

	// mapping of allowable non-move input to their corresponding functions
	// eg) "q" -> quit()
	std::map<std::string, std::function<void()>> input_map;


	virtual void render_start_screen();
	virtual void render_end_screen();
	std::string won_lost_statement();
	void game_loop();
	void reset();
	virtual void update();    // updates availableMoveList
	virtual void render();
	void new_game();
	void quit();
	void render_board();
	void render_options();
	void switch_players();
	virtual std::string get_user_input();
	void process_input();
	void process_input(std::string input);
	void random_move();
	void execute_move(const Move move, const bool is_new_move = true);
	void undo();
	void redo();
	std::string ai_input();
	void alter_players();
	static bool is_int(std::string& s);
};
