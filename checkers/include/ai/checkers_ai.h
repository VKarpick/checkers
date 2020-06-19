/*-------------------------------------------------------------------------------------------------
 Determines which move the AI will select in a Checkers game.

 Piggybacks off of action selection already developed for training the AI in CheckersEnvironment.
-------------------------------------------------------------------------------------------------*/


#pragma once
#include "checkers_environment.h"
#include "checkers_trainer.h"
#include "../reinforcement_learning/policy.h"


class CheckersAI {
public:
	CheckersAI(const int max_depth = constants::default_max_depth);


	Move action_selection(const Checkerboard checkerboard, const CheckersPlayer player);


private:
	const int max_depth_;    // for minimax
	std::shared_ptr<CheckersEnvironment> checkers_environment_;
	std::vector<std::shared_ptr<Player>> players_;
	std::shared_ptr<MinimaxPolicy> minimax_policy_;
};
