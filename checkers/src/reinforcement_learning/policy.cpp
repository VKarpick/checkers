#include "policy.h"
#include <iostream>

Policy::Policy() {}
Policy::Policy(Environment* env) { env_ = env; }


RandomWalkPolicy::RandomWalkPolicy(Environment* env) : Policy(env) {}

Action* RandomWalkPolicy::action_selection(State* state) {
	std::vector<Action*> actions = env_->actions();
	if (!actions.empty()) {
		int random_index = rand() % actions.size();
		return actions[random_index];
	}
	return nullptr;
}


MinimaxPolicy::MinimaxPolicy(Environment* env, Estimator* estimator, Player* max_player, int max_depth) {
	env_ = env;
	estimator_ = estimator;
	max_player_ = max_player;
	max_depth_ = max_depth;

	minimax_ = new Minimax<StateActionPair*>(
		[this](Node<StateActionPair*>* node) { return evaluate(node); }, 
		max_depth_, 
		[this](Node<StateActionPair*>* node) { extend(node); }
	);
}

Action* MinimaxPolicy::action_selection(State* state) {
	// StateActionPair of node has to reflect current state to provide accurate action selection
	if (node_ == nullptr || node_->data()->state != state) reset_node(state);

	node_ = minimax_->minimax_node(node_, 0, node_->data()->state->current_player == max_player_);
	return node_->data()->action;
}

void MinimaxPolicy::reset_node(State* state) {
	// nodes have StateActionPair as type to provide easy retrieval of action from child node that minimax gives highest value to
	std::cout << "reset node" << std::endl;
	node_ = new Node<StateActionPair*>(new StateActionPair{ state, nullptr });
}

// a state's value is the estimator's predicted value for that state
double MinimaxPolicy::evaluate(Node<StateActionPair*>* node) {
	std::vector<double> features = env_->featurize(node->data()->state);
	return estimator_->predict(features);
}

// add children for every possible action from the current state
void MinimaxPolicy::extend(Node<StateActionPair*>* node) {
	for (Action* action : env_->actions(node->data()->state)) {
		State* next_state = env_->step(node->data()->state, action);
		Node<StateActionPair*>* child = new Node<StateActionPair*>(new StateActionPair{ next_state, action });
		node->add_child(child);
	}
}
