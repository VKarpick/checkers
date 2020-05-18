#pragma once

#include "environment.h"
#include "estimator.h"
#include "../tree/minimax.h"
#include <random>

class Policy {
public:
	Policy(Environment* env);
	virtual std::any action_selection(State* state) = 0;

protected:
	Environment* env_;
};

class RandomWalkPolicy : public Policy {
	std::any action_selection(State* state);
};

class MinimaxPolicy : public Policy {
public:
	MinimaxPolicy(Environment* env, Estimator* estimator, int max_depth);

	std::any action_selection(State* state);
	void reset_node(State* state);

private:
	Estimator* estimator_;
	int max_depth_;
	Node<State*>* node_;
	Minimax<State*>* minimax_;
};
