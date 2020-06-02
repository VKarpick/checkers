#pragma once

#include <string>

#include "../checkers.h"
#include "../reinforcement_learning/environment.h"



class CheckersEnvironment : public Environment {
public:
	std::vector<std::shared_ptr<Player>> get_players();


	std::shared_ptr<State> reset() override;
	std::shared_ptr<State> step(std::shared_ptr<Action> action) override;
	std::shared_ptr<State> step(std::shared_ptr<State> state, std::shared_ptr<Action> action) override;
	std::vector<std::shared_ptr<Action>> get_actions() override;
	std::vector<std::shared_ptr<Action>> get_actions(std::shared_ptr<State> state) override;
	std::vector<double> featurize(std::shared_ptr<State> state) override;


	Checkerboard checkerboard_from_state(std::shared_ptr<State> state);
	std::shared_ptr<Player> opponent_from_state(std::shared_ptr<State> state);
	CheckersPlayer checkers_player_from_state(std::shared_ptr<State> state);
	CheckersPlayer checkers_player_from_player(std::shared_ptr<Player> player);


private:
	std::shared_ptr<State> state_{ nullptr };
	int n_plays_{ 0 };
	const int max_plays{ 150 };    // simple way of determining draws, terminate after given number of plays
	std::vector<std::shared_ptr<Player>> players_{
		std::make_shared<Player>(Player{CheckersPlayer{'r', -1, false}}),    //TODO remove magic variables
		std::make_shared<Player>(Player{CheckersPlayer{'w', 1, false}})
	};
};
