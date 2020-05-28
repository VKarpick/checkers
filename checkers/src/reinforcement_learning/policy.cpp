#include "policy.h"



Policy::Policy() {}
Policy::Policy(std::shared_ptr<Environment> environment) : environment_(environment) {}


std::shared_ptr<Node<StateActionPair>> Policy::getNode() {
	return nullptr;
}



RandomWalkPolicy::RandomWalkPolicy(std::shared_ptr<Environment> environment) : environment_(environment) {}


std::shared_ptr<Action> RandomWalkPolicy::actionSelection(std::shared_ptr<State> state) {
	std::vector<std::shared_ptr<Action>> actions = environment_->getActions();
	if (!actions.empty()) {
		int randomIndex = rand() % actions.size();
		return actions[randomIndex];
	}
	return nullptr;
}



MinimaxPolicy::MinimaxPolicy(std::shared_ptr<Environment> environment, std::shared_ptr<Estimator> estimator, 
	std::shared_ptr<Player> maxPlayer, int maxDepth) :
	environment_(environment),
	estimator_(estimator),
	maxPlayer_(maxPlayer),
	maxDepth_(maxDepth)
{
	minimax_ = std::make_unique<Minimax<StateActionPair>>(Minimax<StateActionPair>(
		[this](std::shared_ptr<Node<StateActionPair>> node) { return computeNodeValue(node); },
		maxDepth_, 
		[this](std::shared_ptr<Node<StateActionPair>> node) { extendTree(node); }
	));
}


std::shared_ptr<Node<StateActionPair>> MinimaxPolicy::getNode() {
	return node_;
}


std::shared_ptr<Action> MinimaxPolicy::actionSelection(std::shared_ptr<State> state) {
	// StateActionPair of node has to reflect current state to provide accurate action selection
	if (node_ == nullptr || node_->getData().state != state) resetNode(state);

	node_ = minimax_->minimaxNode(node_, 0, node_->getData().state->currentPlayer == maxPlayer_);
	return node_->getData().action;
}


void MinimaxPolicy::resetNode(std::shared_ptr<State> state) {
	// nodes have StateActionPair as type to provide easy retrieval of action from child node that minimax gives highest value to
	node_ = std::make_shared<Node<StateActionPair>>(StateActionPair{ state, nullptr });
}


// a state's value is the estimator's predicted value for that state
double MinimaxPolicy::computeNodeValue(std::shared_ptr<Node<StateActionPair>> node) {
	std::vector<double> features = environment_->featurize(node->getData().state);
	return estimator_->predict(features);
}


// add children for every possible action from the current state
void MinimaxPolicy::extendTree(std::shared_ptr<Node<StateActionPair>> node) {
	for (std::shared_ptr<Action> action : environment_->getActions(node->getData().state)) {
		std::shared_ptr<State> next_state = environment_->step(node->getData().state, action);
		std::shared_ptr<Node<StateActionPair>> child = std::make_shared<Node<StateActionPair>>(StateActionPair{ next_state, action });
		node->addChild(child);
	}
}
