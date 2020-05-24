#pragma once

#include <iostream>

#include "policy.h"



//TODO prediction vs control
class TDLambda {
public:
	TDLambda();
	TDLambda(Environment* environment, Estimator* estimator, Policy* policy, double discountFactor = 1.0, double traceDecay = 1.0);


	void train(int nEpisodes = 1, bool isPrintingUpdates = false);


protected:
	Environment* environment_{ nullptr };
	Estimator* estimator_{ nullptr };
	Policy* policy_{ nullptr };
	double discountFactor_{ 1.0 };    // gamma
	double traceDecay_{ 1.0 };    // lambda
};



class TDLeaf : public TDLambda {
public:
	TDLeaf(Environment* environment, Estimator* estimator, Player* maxPlayer, int maxDepth = 1,
		double discountFactor = 1.0, double traceDecay = 0.86);
};
