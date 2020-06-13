#include "ai/checkers_ai.h"



CheckersAI::CheckersAI() {
	checkers_environment_ = std::make_shared<CheckersEnvironment>();
	state_ = checkers_environment_->reset();
	std::vector<double> weights{ CheckersTrainer::read_weights(weights_file) };
	estimator_ = std::make_shared<TDEstimator>(TDEstimator(0.0, weights));
	players_ = checkers_environment_->get_players();
	minimax_policy_ = std::make_shared<MinimaxPolicy>(MinimaxPolicy(checkers_environment_, estimator_, players_[0], max_depth_));
}


Move CheckersAI::action_selection(Checkerboard checkerboard, CheckersPlayer current_player) {
	std::shared_ptr<Player> player{ (current_player == std::any_cast<CheckersPlayer>(players_[0]->player)) ? players_[0] : players_[1] };
	state_ = std::make_shared<State>(State{ checkerboard, 0, false, player });
	checkers_environment_->set_state(state_);
	std::shared_ptr<Action> action{ minimax_policy_->action_selection(state_) };
	return std::any_cast<Move>(action->action);
}
