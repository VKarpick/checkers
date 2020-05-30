#pragma once

#include <any>
#include <memory>
#include <vector>



// only necessary for multi-player games
// allow flexibility in defining players while passing around smart pointers to struct
struct Player {
	std::any player;
};



// allow flexibility in defining actions while passing around smart pointers to struct
struct Action {
	std::any action;
};



// allow flexibility in defining observations
// optional current_player member for multi-player games
struct State {
	std::any observation;
	double reward{ 0.0 };
	bool is_terminal{ false };
	std::shared_ptr<Player> current_player{ std::make_shared<Player>(Player{}) };
};



// abstract base class to be inherited from
class Environment {
public:
	// reset environment to a starting state, returning the state
	virtual std::shared_ptr<State> reset() = 0;

	// return the new state that results from taking a given action in the current environment state
	virtual std::shared_ptr<State> step(std::shared_ptr<Action> action) = 0;

	// return the new state that results from taking a given action from a given state
	virtual std::shared_ptr<State> step(std::shared_ptr<State> state, std::shared_ptr<Action> action) = 0;

	// return a vector of all available actions in the current environment state
	virtual std::vector<std::shared_ptr<Action>> get_actions() = 0;

	// return a vector of all available actions available from a given state
	virtual std::vector<std::shared_ptr<Action>> get_actions(std::shared_ptr<State> state) = 0;

	// return a feature vector representing a given state
	virtual std::vector<double> featurize(std::shared_ptr<State> state) = 0;
};
