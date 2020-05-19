#pragma once

#include <random>

#include "environment.h"
#include "estimator.h"
#include "../tree/minimax.h"


struct StateActionPair {
	State* state;
	Action* action;
};


// generic policy to be inherited from
class Policy {
public:
	Policy(Environment* env);
	virtual Action* action_selection(State* state) = 0;

protected:
	Environment* env_;
};


// choose actions entirely at random
class RandomWalkPolicy : public Policy {
	Action* action_selection(State* state) override;
};


// use minimax to choose actions
class MinimaxPolicy : public Policy {
public:
	MinimaxPolicy(Environment* env, Estimator* estimator, int max_depth, Player* max_player);

	Action* action_selection(State* state) override;
	void reset_node(State* state);

private:
	Estimator* estimator_;
	int max_depth_;
	Node<StateActionPair*>* node_{ nullptr };
	Minimax<StateActionPair*>* minimax_;
	std::any max_player_;

	// functions to be passed to minimax algorithm
	double evaluate(Node<StateActionPair*>* node);
	double extend(Node<StateActionPair*>* node);
};
