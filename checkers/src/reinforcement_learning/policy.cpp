#include "policy.h"


Policy::Policy(Environment* env) { env_ = env; }


Action* RandomWalkPolicy::action_selection(State* state) {
	std::vector<Action*> actions = env_->actions();
	random_shuffle(actions.begin(), actions.end());
	return actions[0];
}


MinimaxPolicy::MinimaxPolicy(Environment* env, Estimator* estimator, int max_depth, Player* max_player) {
	env_ = env;
	estimator_ = estimator;
	max_depth_ = max_depth;
	minimax_ = new Minimax(evaluate, max_depth_, extend);
	max_player_ = max_player;
}

Action* MinimaxPolicy::action_selection(State* state) {
	if (node_->data()->state != state) reset_node(state);

	node_ = minimax_node(node_, 0, node_->data()->state->current_player == max_player);
	return node_->data()->action;
}

void MinimaxPolicy::reset_node(State* state) {
	node_ = new Node<StateActionPair*>(state, nullptr);
}

double MinimaxPolicy::evaluate(Node<StateActionPair*>* node) {
	std::vector<double> features = env_->featurize(node->data()->state);
	return estimator_->predict(features);
}

void MinimaxPolicy::extend(Node<StateActionPair*>* node) {
	for (Action* action : env_->actions()) {
		State* next_state = env_->step(node->data()->state, action);
		Node<StateActionPair*> child = new Node<StateActionPair*>(next_state, action);
		node->add_child(child);
	}
}
