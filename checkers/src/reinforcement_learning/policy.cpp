#include "policy.h"

Policy::Policy(Environment* env) { env_ = env; }


std::any RandomWalkPolicy::action_selection(State* state) {
	std::vector<std::any> actions = env_->actions();
	random_shuffle(actions.begin(), actions.end());
	return actions[0];
}

MinimaxPolicy::MinimaxPolicy(Environment* env, Estimator* estimator, int max_depth) {
	env_ = env ;
	estimator_ = estimator;
	max_depth_ = max_depth;
	//double TDEstimator::predict(std::vector<double> features) {
	//virtual std::vector<double> featurize(State * state) = 0;
	double evaluate(Node<State*>* node) {
		std::vector<double> features = env_->featurize(node->data());
		return estimator_.predict(features);
	}
	void extend(Node<State*>* node) {
		for (auto action : env_->actions()) {
			State* next_state = env_->step(node->data(), action);
			Node<State*> child = new Node<State*>(next_state);
		}
	}
	minimax_ = new Minimax(evaluate, max_depth, extend);

	/*Minimax(std::function<double(Node<T>*)> evaluate_node, int init_max_depth = 0,
		const std::function<void(Node<T>*)> extend_tree = [](Node<T>*) {}) {*/
}

std::any MinimaxPolicy::action_selection(State* state) {
	return 0;
}

void MinimaxPolicy::reset_node(State* state) {
	node_ = new Node<State*>(state);
}
