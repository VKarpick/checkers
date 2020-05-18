#pragma once

#include "environment.h"
#include "estimator.h"
#include "../tree/minimax.h"
#include <random>


struct StateActionPair {
	State* state;
	Action* action;
};


class Policy {
public:
	Policy(Environment* env);
	virtual Action* action_selection(State* state) = 0;

protected:
	Environment* env_;
};


class RandomWalkPolicy : public Policy {
	Action* action_selection(State* state) override;
};


class MinimaxPolicy : public Policy {
public:
	MinimaxPolicy(Environment* env, Estimator* estimator, int max_depth, Player* max_player);

	Action* action_selection(State* state) override;
	void reset_node(State* state);

private:
	Estimator* estimator_;
	int max_depth_;
	Node<StateActionPair*>* node_;
	Minimax<State*>* minimax_;
	std::any max_player_;

	double evaluate(Node<StateActionPair*>* node);
	double extend(Node<StateActionPair*>* node);
};
