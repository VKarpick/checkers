#pragma once

#include <iostream>

#include "policy.h"


//TODO prediction vs control
class TDLambda {
public:
	TDLambda();
	TDLambda(Environment* env, Estimator* estimator, Policy* policy, double discount_factor = 1, double trace_decay = 0.86);

	void train(int n_episodes = 1, bool print_update = false);

protected:
	Environment* env_;
	Estimator* estimator_;
	Policy* policy_;
	double discount_factor_;    // gamma
	double trace_decay_;    // lambda
};


class TDLeaf : public TDLambda {
public:
	TDLeaf(Environment* env, Estimator* estimator, Player* max_player, int max_depth = 1,
		double discount_factor = 1, double trace_decay = 0.86);
};
