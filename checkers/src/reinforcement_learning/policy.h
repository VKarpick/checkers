#pragma once

#include <memory>
#include <random>

#include "environment.h"
#include "estimator.h"
#include "../tree/minimax.h"



struct StateActionPair {
	/*State* state;
	Action* action;*/
	std::shared_ptr<State> state;
	std::shared_ptr<Action> action;
};



// abstract base class to be inherited from
class Policy {
public:
	Policy();
	Policy(Environment* environment);


	virtual std::shared_ptr<Node<StateActionPair>> getNode();    // only necessary for MinimaxPolicy
	
	
	//virtual Action* actionSelection(State* state) = 0;
	virtual std::shared_ptr<Action> actionSelection(std::shared_ptr<State> state) = 0;


protected:
	Environment* environment_{ nullptr };
};



// choose actions entirely at random
class RandomWalkPolicy : public Policy {
public:
	RandomWalkPolicy(Environment* env);


	//Action* actionSelection(State* state) override;
	std::shared_ptr<Action> actionSelection(std::shared_ptr<State> state) override;

	
private:
	Environment* environment_{ nullptr };
};



// use minimax to choose actions
class MinimaxPolicy : public Policy {
public:
	//MinimaxPolicy(Environment* environment, Estimator* estimator, Player* maxPlayer, int maxDepth = 1);
	MinimaxPolicy(Environment* environment, Estimator* estimator, std::shared_ptr<Player> maxPlayer, int maxDepth = 1);


	std::shared_ptr<Node<StateActionPair>> getNode();


	//Action* actionSelection(State* state) override;
	std::shared_ptr<Action> actionSelection(std::shared_ptr<State> state) override;
	//void resetNode(State* state);
	void resetNode(std::shared_ptr<State> state);


private:
	Environment* environment_{ nullptr };
	Estimator* estimator_{ nullptr };
	//Player* maxPlayer_{ nullptr };
	std::shared_ptr<Player> maxPlayer_{ nullptr };
	int maxDepth_{ 1 };
	std::shared_ptr<Node<StateActionPair>> node_{ nullptr };
	Minimax<StateActionPair>* minimax_{ nullptr };

	// functions to be passed to minimax algorithm
	double computeNodeValue(std::shared_ptr<Node<StateActionPair>> node);
	void extendTree(std::shared_ptr<Node<StateActionPair>> node);
};
