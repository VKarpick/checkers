#pragma once
#include "../checkers.h"
#include "checkers_environment.h"
#include "checkers_trainer.h"
#include "../reinforcement_learning/policy.h"



class CheckersAI {
public:
	Move action_selection();
	void process_move(Move move);


private:
	std::shared_ptr<CheckersEnvironment> checkers_environment_{ std::make_shared<CheckersEnvironment>() };
	std::shared_ptr<State> state{ checkers_environment_->reset() };
	std::vector<double> weights{ CheckersTrainer::read_weights("weights.dat") };
	std::shared_ptr<TDEstimator> estimator_{ 
		std::make_shared<TDEstimator>(TDEstimator(0.0, weights))
	};
	std::vector<std::shared_ptr<Player>> players_{ checkers_environment_->get_players() };
	int max_depth_{ 6 };
	std::shared_ptr<MinimaxPolicy> minimax_policy_{ 
		std::make_shared<MinimaxPolicy>(MinimaxPolicy(checkers_environment_, estimator_, players_[0], max_depth_))
	};
};
