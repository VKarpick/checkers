#include "policy.h"



Policy::Policy() {}
Policy::Policy(Environment* environment) : environment_(environment) {}


std::shared_ptr<Node<StateActionPair>> Policy::getNode() {
	return nullptr;
}



RandomWalkPolicy::RandomWalkPolicy(Environment* environment) : environment_(environment) {}


Action* RandomWalkPolicy::actionSelection(State* state) {
	std::vector<Action*> actions = environment_->getActions();
	if (!actions.empty()) {
		int randomIndex = rand() % actions.size();
		return actions[randomIndex];
	}
	return nullptr;
}



MinimaxPolicy::MinimaxPolicy(Environment* environment, Estimator* estimator, Player* maxPlayer, int maxDepth) :
	environment_(environment),
	estimator_(estimator),
	maxPlayer_(maxPlayer),
	maxDepth_(maxDepth)
{
	minimax_ = new Minimax<StateActionPair>(
		[this](std::shared_ptr<Node<StateActionPair>> node) { return computeNodeValue(node); },
		maxDepth_, 
		[this](std::shared_ptr<Node<StateActionPair>> node) { extendTree(node); }
	);
}


std::shared_ptr<Node<StateActionPair>> MinimaxPolicy::getNode() {
	return node_;
}


Action* MinimaxPolicy::actionSelection(State* state) {
	// StateActionPair of node has to reflect current state to provide accurate action selection
	if (node_ == nullptr || node_->getData().state != state) resetNode(state);

	node_ = minimax_->minimaxNode(node_, 0, node_->getData().state->currentPlayer == maxPlayer_);
	return node_->getData().action;
}


void MinimaxPolicy::resetNode(State* state) {
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
	for (Action* action : environment_->getActions(node->getData().state)) {
		State* next_state = environment_->step(node->getData().state, action);
		std::shared_ptr<Node<StateActionPair>> child = std::make_shared<Node<StateActionPair>>(StateActionPair{ next_state, action });
		node->addChild(child);
	}
}
