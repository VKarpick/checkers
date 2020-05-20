#pragma once

#include <random>

#include "environment.h"
#include "estimator.h"
#include "../tree/minimax.h"


struct StateActionPair {
	State* state;
	Action* action;
};


// abstract base class to be inherited from
class Policy {
public:
	Policy();
	Policy(Environment* env);
	virtual Node<StateActionPair*>* node();    // only necessary for MinimaxPolicy
	virtual Action* action_selection(State* state) = 0;

protected:
	Environment* env_;
};


// choose actions entirely at random
class RandomWalkPolicy : public Policy {
public:
	RandomWalkPolicy(Environment* env);
	Action* action_selection(State* state) override;
};


// use minimax to choose actions
class MinimaxPolicy : public Policy {
public:
	MinimaxPolicy(Environment* env, Estimator* estimator, Player* max_player, int max_depth = 1);

	// getter
	Node<StateActionPair*>* node();

	Action* action_selection(State* state) override;
	void reset_node(State* state);

private:
	Estimator* estimator_;
	Player* max_player_;
	int max_depth_;
	Node<StateActionPair*>* node_{ nullptr };
	Minimax<StateActionPair*>* minimax_;

	// functions to be passed to minimax algorithm
	double evaluate(Node<StateActionPair*>* node);
	void extend(Node<StateActionPair*>* node);
};
