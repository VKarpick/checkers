#pragma once
#include <numeric>
#include <random>
#include "minimax.h"

class Policy {
public:
	Policy(CheckersEnv* env);
	Policy() {}
	virtual vector<string> Action();
	virtual void Reset();

protected:
	CheckersEnv* env_ = new CheckersEnv;
};

class RandomWalk : public Policy {
public:
	RandomWalk(CheckersEnv* env);
	vector<string> Action();
};

class MinimaxPolicy : public Policy {
public:
	MinimaxPolicy(CheckersEnv* env, Estimator* estimator, int max_depth);
	Node<vector<string>>* node;
	vector<string> Action();
	void Reset();

private:
	Estimator* estimator_;
	int max_depth_;

};

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
