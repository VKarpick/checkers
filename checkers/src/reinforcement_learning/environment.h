#pragma once

#include <vector>
#include <any>


struct Player {
	std::any player;
};


struct Action {
	std::any action;
};


struct State {
	std::any observation;
	float reward;
	bool terminal;
	Player* current_player;
};


class Environment {
public:
	virtual State* reset() = 0;
	virtual State* step(std::any action) = 0;
	virtual State* step(State* state, std::any action) = 0;
	virtual std::vector<Action*> actions() = 0;
	virtual std::vector<double> featurize(State* state) = 0;
};
