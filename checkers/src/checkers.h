#pragma once

#include <algorithm>
#include <functional>
#include <map>
#include <random>

#include "checkerboard.h"
#include "checkerboard_moves.h"
#include "checkers_player.h"
#include "checkers_structs.h"
#include "constants.h"
#include "ai/checkers_ai.h"



class Checkers {
public:
	Checkers();


	std::vector<Move> get_available_moves();


	void play();
	void reset();
	void update();    // updates availableMoveList
	void quit();


private:
	Checkerboard checkerboard_;
	CheckersAI checkers_ai_;
	std::vector<CheckersPlayer> players_{ CheckersPlayer{'r', -1, true}, CheckersPlayer{'w', 1, false} };
	CheckersPlayer& current_player_{ players_[0] };
	CheckersPlayer& opponent_{ players_[1] };
	std::vector<Move> available_moves_{};
	std::vector<Move> previous_moves_{};
	std::vector<Move> redo_moves_{};

	// mapping of allowable non-move input to their corresponding functions
	// eg) "q" -> quit()
	std::map<std::string, std::function<void()>> input_map;


	//bool isCrowningMove(char pieceChar, BoardPosition boardPosition);
	void switch_players();
	void render();
	std::string get_user_input();
	void process_input(std::string input);
	void random_move();
	void make_move(Move move, bool is_new_move = true);
	void undo();
	void redo();
	std::string ai_input();
};
