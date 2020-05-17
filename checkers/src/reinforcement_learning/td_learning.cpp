#include "td_learning.h"

void td_lambda(CheckersEnv* env, Estimator* estimator, Policy* policy, double discount_factor, double trace_decay) {

	vector<string> state = env->Reset();
	estimator->Reset();
	tuple<vector<string>, double, bool> subsequents;

	do {
		vector<string> action{ policy->Action() };
		subsequents = env->Step(action);    // next_state, reward, terminal
		vector<int> next_state_features{ env->BoardToFeatures(get<0>(subsequents)) };
		double target{ get<1>(subsequents) + discount_factor * estimator->Predict(next_state_features) };
		vector<int> state_features{ env->BoardToFeatures(state) };
		double estimate{ estimator->Predict(state_features) };
		estimator->Update(target, estimate, state_features, discount_factor, trace_decay);

		state = get<0>(subsequents);

	} while (!get<2>(subsequents));
}

TDLeaf::TDLeaf(CheckersEnv* env, int feature_size, int max_depth, double step_size, double discount_factor, double trace_decay) {

	env_ = env;
	vector<double> weights(feature_size, 0);
	estimator_ = new Estimator(weights, step_size);
	policy_ = new MinimaxPolicy(env, estimator_, max_depth);
	discount_factor_ = discount_factor;
	trace_decay_ = trace_decay;
}

void TDLeaf::Train(int n_episodes) {
	for (int i_episode = 0; i_episode < n_episodes; ++i_episode) {
		env_->Reset();
		policy_->Reset();
		cout << "Episode:  " << (i_episode + 1) << " of " << n_episodes << endl;
		td_lambda(env_, estimator_, policy_, discount_factor_, trace_decay_);
	}

	cout << "End weights" << endl;
	for (double weight : estimator_->weights()) {
		cout << weight << endl;
	}
}
