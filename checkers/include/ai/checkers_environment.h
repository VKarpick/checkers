/*-------------------------------------------------------------------------------------------------
 Environment used for training Checkers AI.

 State:  instance of Checkerboard
 Action:  moves available in an instance of Checkerboard
 Reward:  0.001 if first player wins, -0.001 if second players, 0 for anything else
 Termination:  someone wins or the game reaches a maximal number of turns

 Checkerboard features:
 - Current player (1 = first player, 0 = second)
 - For each of the accessible squares on the board:
     * Square is occupied by a regular piece of the first player
	 * Square is occupied by a king of the first player
	 * Square is occupied by a regular piece of the second player
	 * Square is occupied by a king of the first player
 - Total # of regular (non-king) pieces for each player
 - Total # of kings for each player
 - Total # of pieces for each player
 - # of trapped kings for each player (kings that can't move without being captured)
 - # of runaway pieces for each player (pieces with unimpeded paths to be crowned)
     allowed for some false positives to decrease time spent counting runaways
-------------------------------------------------------------------------------------------------*/


#pragma once

#include "checkerboard_moves.h"
#include "reinforcement_learning/environment.h"


class CheckersEnvironment : public Environment {
public:
	std::shared_ptr<State> reset() override;
	std::shared_ptr<State> step(std::shared_ptr<Action> action) override;
	std::shared_ptr<State> step(std::shared_ptr<State> state, std::shared_ptr<Action> action) override;
	std::vector<std::shared_ptr<Action>> get_actions() override;
	std::vector<std::shared_ptr<Action>> get_actions(std::shared_ptr<State> state) override;
	std::vector<double> featurize(const std::shared_ptr<State> state) override;


	std::vector<std::shared_ptr<Player>> get_players();


	void set_state(const std::shared_ptr<State> state);


private:
	std::shared_ptr<State> state_{ nullptr };
	int n_plays_{ 0 };
	const int max_plays_{ 100 };    // simple way of determining draws, terminate after given number of plays
	const std::vector<std::shared_ptr<Player>> players_{
		std::make_shared<Player>(Player{CheckersPlayer{constants::pieces[0], constants::player_directions[0], false}}),
		std::make_shared<Player>(Player{CheckersPlayer{constants::pieces[1], constants::player_directions[1], false}})
	};


	Checkerboard checkerboard_from_state(const std::shared_ptr<State> state);
	std::shared_ptr<Player> opponent_from_state(const std::shared_ptr<State> state);
	CheckersPlayer checkers_player_from_state(const std::shared_ptr<State> state);
	CheckersPlayer checkers_player_from_player(const std::shared_ptr<Player> player);
	bool is_trapped_king(const Checkerboard board, const BoardPosition piece_position, 
		const CheckersPlayer current_player, const CheckersPlayer opponent);
	bool can_be_jumped(const Checkerboard board, const BoardPosition piece_position, 
		const CheckersPlayer current_player, const CheckersPlayer opponent);
	bool avoid_encounters(Checkerboard board, const BoardPosition piece_position, const CheckersPlayer current_player);
	bool is_runaway(const Checkerboard board, const BoardPosition piece_position, 
		const CheckersPlayer current_player, const CheckersPlayer opponent);
};
