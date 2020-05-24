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
	Policy(Environment* environment);


	virtual Node<StateActionPair*>* getNode();    // only necessary for MinimaxPolicy
	
	
	virtual Action* actionSelection(State* state) = 0;


protected:
	Environment* environment_;
};



// choose actions entirely at random
class RandomWalkPolicy : public Policy {
public:
	RandomWalkPolicy(Environment* env);


	Action* actionSelection(State* state) override;
};



// use minimax to choose actions
class MinimaxPolicy : public Policy {
public:
	MinimaxPolicy(Environment* environment, Estimator* estimator, Player* maxPlayer, int maxDepth = 1);


	Node<StateActionPair*>* getNode();


	Action* actionSelection(State* state) override;
	void resetNode(State* state);


private:
	Estimator* estimator_;
	Player* maxPlayer_;
	int maxDepth_;
	Node<StateActionPair*>* node_{ nullptr };
	Minimax<StateActionPair*>* minimax_;

	// functions to be passed to minimax algorithm
	double computeNodeValue(Node<StateActionPair*>* node);
	void extendTree(Node<StateActionPair*>* node);
};
