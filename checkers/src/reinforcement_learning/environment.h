#pragma once

#include <any>
#include <vector>


// only necessary for multi-player games
// allow flexibility in defining players while passing around pointers to struct
struct Player {
	Player() { player = 0; }
	Player(std::any p) { player = p; }
	std::any player;
};


// allow flexibility in defining actions while passing around pointers to struct
struct Action {
	Action(std::any a) { action = a; }
	std::any action;
};


// allow flexibility in defining observations
// optional current_player member for multi-player games
struct State {
	State(std::any o, double r, bool t, Player* cp = new Player()) {
		observation = o;
		reward = r;
		terminal = t;
		current_player = cp;
	}

	std::any observation;
	double reward;
	bool terminal;
	Player* current_player{ new Player() };
};


// generic environment to be inherited from
class Environment {
public:
	// reset environment to a starting state, returning the state
	virtual State* reset() = 0;

	// return the new state that results from taking a given action in the current environment state
	virtual State* step(Action* action) = 0;

	// return the new state that results from taking a given action from a given state
	virtual State* step(State* state, Action* action) = 0;

	// vector of all available actions in the current environment state
	virtual std::vector<Action*> actions() = 0;

	// vector of all available actions available from a given state
	virtual std::vector<Action*> actions(State* state) = 0;

	// convert a state to a feature vector
	virtual std::vector<double> featurize(State* state) = 0;
};
