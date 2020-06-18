#pragma once

#include <string>

#include "../checkerboard_moves.h"
#include "../reinforcement_learning/environment.h"



class CheckersEnvironment : public Environment {
public:
	std::shared_ptr<State> reset() override;
	std::shared_ptr<State> step(std::shared_ptr<Action> action) override;
	std::shared_ptr<State> step(std::shared_ptr<State> state, std::shared_ptr<Action> action) override;
	std::vector<std::shared_ptr<Action>> get_actions() override;
	std::vector<std::shared_ptr<Action>> get_actions(std::shared_ptr<State> state) override;
	std::vector<double> featurize(std::shared_ptr<State> state) override;


	std::vector<std::shared_ptr<Player>> get_players();


	void set_state(std::shared_ptr<State> state);


private:
	std::shared_ptr<State> state_{ nullptr };
	int n_plays_{ 0 };
	const int max_plays_{ 100 };    // simple way of determining draws, terminate after given number of plays
	std::vector<std::shared_ptr<Player>> players_{
		std::make_shared<Player>(Player{CheckersPlayer{constants::pieces[0], -1, false}}),
		std::make_shared<Player>(Player{CheckersPlayer{constants::pieces[1], 1, false}})
	};


	Checkerboard checkerboard_from_state(std::shared_ptr<State> state);
	std::shared_ptr<Player> opponent_from_state(std::shared_ptr<State> state);
	CheckersPlayer checkers_player_from_state(std::shared_ptr<State> state);
	CheckersPlayer checkers_player_from_player(std::shared_ptr<Player> player);
	bool is_trapped_king(Checkerboard board, BoardPosition piece_position, CheckersPlayer current_player, CheckersPlayer opponent);
	bool can_be_jumped(Checkerboard board, BoardPosition piece_position, CheckersPlayer current_player, CheckersPlayer opponent);
	bool avoid_encounters(Checkerboard board, BoardPosition piece_position, CheckersPlayer current_player);
	bool is_runaway(Checkerboard board, BoardPosition piece_position, CheckersPlayer current_player, CheckersPlayer opponent);
};
