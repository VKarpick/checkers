#pragma once
#include "checkers_environment.h"
#include "checkers_trainer.h"
#include "../reinforcement_learning/policy.h"



const std::string weights_file{ "..\\checkers\\data\\weights.dat" };



class CheckersAI {
public:
	CheckersAI();


	Move action_selection(Checkerboard checkerboard, CheckersPlayer player);


private:
	std::shared_ptr<CheckersEnvironment> checkers_environment_;
	std::shared_ptr<State> state_;
	std::shared_ptr<TDEstimator> estimator_;
	std::vector<std::shared_ptr<Player>> players_;
	int max_depth_{ 6 };
	std::shared_ptr<MinimaxPolicy> minimax_policy_;
};
