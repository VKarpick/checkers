#include "reinforcement_learning/policy.h"


Policy::Policy() {}
Policy::Policy(std::shared_ptr<Environment> environment) : environment_(environment) {}


std::shared_ptr<Node<StateActionPair>> Policy::get_node() {
	return nullptr;
}


RandomWalkPolicy::RandomWalkPolicy(std::shared_ptr<Environment> environment) : environment_(environment) {}


std::shared_ptr<Action> RandomWalkPolicy::action_selection(std::shared_ptr<State> state) {
	const std::vector<std::shared_ptr<Action>> actions{ environment_->get_actions() };

	if (!actions.empty()) {
		int random_index = rand() % actions.size();
		return actions[random_index];
	}
	else {
		return nullptr;
	}
}


MinimaxPolicy::MinimaxPolicy(std::shared_ptr<Environment> environment, std::shared_ptr<Estimator> estimator, 
	std::shared_ptr<Player> maxPlayer, int maxDepth) :
	environment_(environment),
	estimator_(estimator),
	max_player_(maxPlayer),
	max_depth_(maxDepth)
{
	minimax_ = std::make_unique<Minimax<StateActionPair>>(Minimax<StateActionPair>(

		// a state's value is the estimator's predicted value for that state
		[environment, estimator](std::shared_ptr<Node<StateActionPair>> node) {
			std::vector<double> features = environment->featurize(node->get_data().state);
			return estimator->predict(features);
		},
		
		max_depth_, 

		// add children for every possible action from the current state
		[environment](std::shared_ptr<Node<StateActionPair>> node) {
			for (std::shared_ptr<Action> action : environment->get_actions(node->get_data().state)) {
				std::shared_ptr<State> next_state{ environment->step(node->get_data().state, action) };
				std::shared_ptr<Node<StateActionPair>> child{ 
					std::make_shared<Node<StateActionPair>>(StateActionPair{ next_state, action }) };
				node->add_child(child);
			}
		}
	));
}


std::shared_ptr<Node<StateActionPair>> MinimaxPolicy::get_node() {
	return node_;
}


std::shared_ptr<Action> MinimaxPolicy::action_selection(std::shared_ptr<State> state) {
	// StateActionPair of node has to reflect current state to provide accurate action selection
	if (node_ == nullptr || node_->get_data().state != state) {
		reset_node(state);
	}

	node_ = minimax_->minimax_node(node_, 0, node_->get_data().state->current_player == max_player_);
	return node_->get_data().action;
}


void MinimaxPolicy::reset_node(std::shared_ptr<State> state) {
	// nodes have StateActionPair as type to provide easy retrieval of action from child node that minimax gives highest value to
	node_ = std::make_shared<Node<StateActionPair>>(StateActionPair{ state, nullptr });
}
