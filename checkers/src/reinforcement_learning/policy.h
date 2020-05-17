#pragma once

#include "environment.h"
#include "estimator.h"
#include "../tree/minimax.h"
#include <random>

class Policy {
public:
	Policy(Environment* env);
	virtual int action_selection(std::vector<int> actions) = 0;

protected:
	Environment* env_;
};

class RandomWalkPolicy : public Policy {
	int action_selection(std::vector<int> actions);
};

class MinimaxPolicy : public Policy {
public:
	int action_selection(std::vector<int> actions);
	void reset_node();

private:
	Estimator* estimator_;
	int max_depth_;
};
