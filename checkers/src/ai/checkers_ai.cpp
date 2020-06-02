#include "checkers_ai.h"



CheckersAI::CheckersAI() {

	//checkers_environment_ = std::make_shared<CheckersEnvironment>();
	/*state_ = checkers_environment_->reset();
	std::vector<double> weights{ CheckersTrainer::read_weights("weights.dat") };
	estimator_ = std::make_shared<TDEstimator>(TDEstimator(0.0, weights));
	std::vector<std::shared_ptr<Player>> players{ checkers_environment_->get_players() };
	minimax_policy_ = std::make_shared<MinimaxPolicy>(MinimaxPolicy(checkers_environment_, estimator_, players[0], max_depth_));*/
}


Move CheckersAI::action_selection() {
	std::shared_ptr<Action> action{ minimax_policy_->action_selection(state_) };
	return std::any_cast<Move>(action->action);
}


void CheckersAI::process_move(Move move) {
	std::shared_ptr<Action> action{ std::make_shared<Action>(Action{ move }) };
	//state_ = checkers_environment_->step(action);
}
