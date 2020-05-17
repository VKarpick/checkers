#pragma once
#include <numeric>
#include <random>

void td_lambda(CheckersEnv* env, Estimator* estimator, Policy* policy, double discount_factor = 1.0, double trace_decay = 0.86);
//void train_td_leaf(int n_episodes, int max_depth = 6, double step_size = 0.01, double discount_factor = 1.0, double trace_decay = 0.86);

class TDLeaf {
public:
	TDLeaf(CheckersEnv* env, int feature_size, int max_depth = 1,
		double step_size = 0.01, double discount_factor = 1.0, double trace_decay = 0.86);

	void Train(int n_episodes = 1);

private:
	CheckersEnv* env_;
	Estimator* estimator_;
	MinimaxPolicy* policy_;
	double discount_factor_;
	double trace_decay_;

};
