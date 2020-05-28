#pragma once

#include <any>
#include <memory>
#include <vector>



// only necessary for multi-player games
// allow flexibility in defining players while passing around pointers to struct
struct Player {
	std::any player;
};



// allow flexibility in defining actions while passing around pointers to struct
struct Action {
	std::any action;
};



// allow flexibility in defining observations
// optional current_player member for multi-player games
struct State {
	std::any observation;
	double reward{ 0.0 };
	bool isTerminal{ false };
	//Player* currentPlayer{ new Player() };
	std::shared_ptr<Player> currentPlayer{ std::make_shared<Player>(Player{}) };
};



// abstract base class to be inherited from
class Environment {
public:
	// reset environment to a starting state, returning the state
	//virtual State* reset() = 0;
	virtual std::shared_ptr<State> reset() = 0;

	// return the new state that results from taking a given action in the current environment state
	//virtual State* step(Action* action) = 0;
	virtual std::shared_ptr<State> step(std::shared_ptr<Action> action) = 0;

	// return the new state that results from taking a given action from a given state
	//virtual State* step(State* state, Action* action) = 0;
	virtual std::shared_ptr<State> step(std::shared_ptr<State> state, std::shared_ptr<Action> action) = 0;

	// vector of all available actions in the current environment state
	//virtual std::vector<Action*> getActions() = 0;
	virtual std::vector<std::shared_ptr<Action>> getActions() = 0;

	// vector of all available actions available from a given state
	//virtual std::vector<Action*> getActions(State* state) = 0;
	virtual std::vector<std::shared_ptr<Action>> getActions(std::shared_ptr<State> state) = 0;

	// convert a state to a feature vector
	//virtual std::vector<double> featurize(State* state) = 0;
	virtual std::vector<double> featurize(std::shared_ptr<State> state) = 0;
};
