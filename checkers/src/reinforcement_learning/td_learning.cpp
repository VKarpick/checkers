#include "reinforcement_learning/td_learning.h"


TDLambda::TDLambda() {}


TDLambda::TDLambda(std::shared_ptr<Environment> environment, std::shared_ptr<Estimator> estimator, 
	std::shared_ptr<Policy> policy, double discount_factor, double trace_decay) :
	
	environment_(environment),
	estimator_(estimator),
	policy_(policy),
	discount_factor_(discount_factor),
	trace_decay_(trace_decay)
{}


void TDLambda::train(int n_episodes, bool is_printing_updates) {
	for (int episode_no = 0; episode_no < n_episodes; ++episode_no) {
		estimator_->reset_eligibility_trace();
		if (is_printing_updates) {
			std::cout << "Episode:  " << (episode_no + 1) << " of " << n_episodes << std::endl;
		}
		train_single_episode();
	}
}


void TDLambda::train_single_episode() {
	std::shared_ptr<State> state{ environment_->reset() };
	std::shared_ptr<State> next_state{ std::make_shared<State>() };
	
	do {
		std::shared_ptr<Action> action{ policy_->action_selection(state) };
		next_state = environment_->step(action);

		// hacky way to avoid reseting node each time when using MinimaxPolicy
		if (policy_->get_node() != nullptr) {
			next_state = policy_->get_node()->get_data().state;
		}

		const std::vector<double> state_features{ environment_->featurize(state) };
		const std::vector<double> next_state_features{ environment_->featurize(next_state) };

		const double target{ next_state->reward + discount_factor_ * estimator_->predict(next_state_features) };
		const double estimate{ estimator_->predict(state_features) };
		estimator_->update(target, estimate, state_features, discount_factor_, trace_decay_);

		state = next_state;

	} while (!next_state->is_terminal);
}


TDLeaf::TDLeaf(std::shared_ptr<Environment> environment, std::shared_ptr<Estimator> estimator, std::shared_ptr<Player> max_player,
	int max_depth, double discount_factor, double trace_decay) {
	
	environment_ = environment;
	estimator_ = estimator;
	policy_ = std::make_shared<MinimaxPolicy>(MinimaxPolicy(environment_, estimator_, max_player, max_depth));
	discount_factor_ = discount_factor;
	trace_decay_ = trace_decay;
}
