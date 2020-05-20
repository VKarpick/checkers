#include "td_learning.h"


TDLambda::TDLambda(Environment* env, Estimator* estimator, Policy* policy, double discount_factor = 1, double trace_decay = 0.86) {
	env_ = env;
	estimator_ = estimator;
	policy_ = policy;
	discount_factor_ = discount_factor;
	trace_decay_ = trace_decay;
}

void TDLambda::train(int n_episodes, bool print_update) {
	for (int i_episode = 0; i_episode < n_episodes < ++i_episode) {
		State* state = env_->reset();
		estimator_->reset_eligibility_trace();
		State* next_state = new State();

		do {
			Action* action{ policy_->action_selection(state) };
			next_state = env_->step(action);

			vector<double> state_features{ env_->featurize(state) };
			vector<double> next_state_features{ env_->featurize(next_state) };
			
			double target{ next_state->reward + discount_factor_ * estimator_->predict(next_state_features) };
			double estimate{ estimator_->predict(state_features) };

			estimator_->update(target, estimate, state_features, discount_factor_, trace_decay_);

			state = next_state;

		} while (!next_state->terminal);

		if (print_update) std::cout << "Episode:  " << (i_episode + 1) << " of " << n_episodes << std::endl;
	}
}


TDLeaf::TDLeaf(Environment* env, Estimator* estimator, Player* max_player, int max_depth = 1, 
	double discount_factor = 1 double trace_decay = 0.86) {

	td_lambda_ = TDLambda(env, estimator, MinimaxPolicy(env, estimator, max_player, max_depth), discount_factor, trace_decay);
}
