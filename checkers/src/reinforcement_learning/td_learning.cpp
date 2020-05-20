#include "td_learning.h"


TDLambda::TDLambda() {}

TDLambda::TDLambda(Environment* env, Estimator* estimator, Policy* policy, double discount_factor, double trace_decay) {
	env_ = env;
	estimator_ = estimator;
	policy_ = policy;
	discount_factor_ = discount_factor;
	trace_decay_ = trace_decay;
}

void TDLambda::train(int n_episodes, bool print_update) {
	for (int i_episode = 0; i_episode < n_episodes; ++i_episode) {
		State* state = env_->reset();
		estimator_->reset_eligibility_trace();
		State* next_state = new State();

		if (print_update) std::cout << "Episode:  " << (i_episode + 1) << " of " << n_episodes << std::endl;

		do {
			Action* action{ policy_->action_selection(state) };
			next_state = env_->step(action);

			// hacky way to avoid reseting node each time when using MinimaxPolicy
			if (policy_->node() != nullptr) next_state = policy_->node()->data()->state;

			std::vector<double> state_features{ env_->featurize(state) };
			std::vector<double> next_state_features{ env_->featurize(next_state) };

			double target{ next_state->reward + discount_factor_ * estimator_->predict(next_state_features) };
			double estimate{ estimator_->predict(state_features) };
			estimator_->update(target, estimate, state_features, discount_factor_, trace_decay_);

			state = next_state;

		} while (!next_state->terminal);
	}
}


TDLeaf::TDLeaf(Environment* env, Estimator* estimator, Player* max_player, int max_depth, double discount_factor, double trace_decay) {
	env_ = env;
	estimator_ = estimator;
	policy_ = new MinimaxPolicy(env_, estimator_, max_player, max_depth);
	discount_factor_ = discount_factor;
	trace_decay_ = trace_decay;
}
