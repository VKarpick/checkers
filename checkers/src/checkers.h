#pragma once

#include <algorithm>
#include <functional>
#include <map>
#include <random>

#include "checkerboard.h"
#include "checkers_player.h"
#include "checkers_structs.h"
#include "constants.h"



//// break these out of class for easy use with checkers_environment
//// returns the moves a piece is allowed to make
//std::vector<Move> piece_moves(Checkerboard board, CheckersPlayer current_player, BoardPosition piece_position, 
//	std::vector<int> row_moves, bool can_capture);
//
//// returns the moves allowable moves from the current board
//std::vector<Move> board_moves(Checkerboard board, CheckersPlayer current_player, CheckersPlayer opponent);



class Checkers {
public:
	Checkers();


	// returns the moves a piece is allowed to make
	static std::vector<Move> piece_moves(Checkerboard board, CheckersPlayer current_player, BoardPosition piece_position,
		std::vector<int> row_moves, bool can_capture);

	// returns the moves allowable moves from the current board
	static std::vector<Move> board_moves(Checkerboard board, CheckersPlayer current_player, CheckersPlayer opponent);


	std::vector<Move> get_available_moves();


	void play();
	void reset();
	void update();    // updates availableMoveList
	void quit();


private:
	Checkerboard checkerboard_;
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
