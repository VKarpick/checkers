/*-------------------------------------------------------------------------------------------------
 TD learning algorithms.

 Currently includes:
     - TDLambda
	 - TDLeaf
-------------------------------------------------------------------------------------------------*/


#pragma once

#include <iostream>

#include "policy.h"


class TDLambda {
public:
	TDLambda();
	TDLambda(std::shared_ptr<Environment> environment, std::shared_ptr<Estimator> estimator, 
		std::shared_ptr<Policy> policy, double discount_factor = 1.0, double trace_decay = 1.0);


	void train(int n_episodes = 1, bool is_printing_updates = false);


protected:
	std::shared_ptr<Environment> environment_{ nullptr };
	std::shared_ptr<Estimator> estimator_{ nullptr };
	std::shared_ptr<Policy> policy_{ nullptr };
	double discount_factor_{ 1.0 };    // gamma
	double trace_decay_{ 1.0 };    // lambda


	void train_single_episode();
};


class TDLeaf : public TDLambda {
public:
	TDLeaf(std::shared_ptr<Environment> environment, std::shared_ptr<Estimator> estimator, 
		std::shared_ptr<Player> maxPlayer, int max_depth = 1, double discount_factor = 1.0, double trace_decay = 0.86);
};
