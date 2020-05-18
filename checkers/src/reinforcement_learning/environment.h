#pragma once

#include <vector>
#include <any>


struct State {
	std::any observation;
	float reward;
	bool terminal;
};


class Environment {
public:
	virtual State* reset() = 0;
	virtual State* step(std::any action) = 0;
	virtual State* step(State* state, std::any action) = 0;
	virtual std::vector<std::any> actions() = 0;
	virtual std::vector<double> featurize(State* state) = 0;
};
