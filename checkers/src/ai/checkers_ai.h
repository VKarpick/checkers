#pragma once
#include "checkers_environment.h"
//#include "checkers_trainer.h"
#include "../reinforcement_learning/policy.h"



class CheckersAI {
public:
	CheckersAI();


	Move action_selection();
	void process_move(Move move);


private:
	CheckersEnvironment checkers_environment_;
	//std::shared_ptr<CheckersEnvironment> checkers_environment_;
	std::shared_ptr<State> state_;
	std::shared_ptr<TDEstimator> estimator_;
	int max_depth_{ 6 };
	std::shared_ptr<MinimaxPolicy> minimax_policy_;
};
