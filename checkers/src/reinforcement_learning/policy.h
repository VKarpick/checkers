#pragma once

#include <memory>
#include <random>

#include "environment.h"
#include "estimator.h"
#include "../tree/minimax.h"



struct StateActionPair {
	std::shared_ptr<State> state;
	std::shared_ptr<Action> action;
};



// abstract base class to be inherited from
class Policy {
public:
	Policy();
	Policy(std::shared_ptr<Environment> environment);


	virtual std::shared_ptr<Node<StateActionPair>> getNode();    // only necessary for MinimaxPolicy
	
	
	virtual std::shared_ptr<Action> actionSelection(std::shared_ptr<State> state) = 0;


protected:
	std::shared_ptr<Environment> environment_{ nullptr };
};



// choose actions entirely at random
class RandomWalkPolicy : public Policy {
public:
	RandomWalkPolicy(std::shared_ptr<Environment> env);


	std::shared_ptr<Action> actionSelection(std::shared_ptr<State> state) override;

	
private:
	std::shared_ptr<Environment> environment_{ nullptr };
};



// use minimax to choose actions
class MinimaxPolicy : public Policy {
public:
	MinimaxPolicy(std::shared_ptr<Environment> environment, std::shared_ptr<Estimator> estimator, 
		std::shared_ptr<Player> maxPlayer, int maxDepth = 1);


	std::shared_ptr<Node<StateActionPair>> getNode();


	std::shared_ptr<Action> actionSelection(std::shared_ptr<State> state) override;
	void resetNode(std::shared_ptr<State> state);


private:
	std::shared_ptr<Environment> environment_{ nullptr };
	std::shared_ptr<Estimator> estimator_{ nullptr };
	std::shared_ptr<Player> maxPlayer_{ nullptr };
	int maxDepth_{ 1 };
	std::shared_ptr<Node<StateActionPair>> node_{ nullptr };
	std::unique_ptr<Minimax<StateActionPair>> minimax_{ nullptr };
};
