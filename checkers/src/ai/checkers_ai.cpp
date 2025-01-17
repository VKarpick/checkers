#include "ai/checkers_ai.h"

#include "ai/checkers_trainer.h"


CheckersAI::CheckersAI(int max_depth) :
	max_depth_(max_depth)
{
	checkers_environment_ = std::make_shared<CheckersEnvironment>();
	checkers_environment_->reset();

	// weights are from the file generated by CheckersTrainer
	// if no weights are found, all weights will be set to 0
	const std::vector<double> weights{ CheckersTrainer::read_weights(constants::weights_file) };
	const std::shared_ptr<TDEstimator> estimator{ std::make_shared<TDEstimator>(TDEstimator(0.0, weights)) };

	players_ = checkers_environment_->get_players();
	
	minimax_policy_ = std::make_shared<MinimaxPolicy>(MinimaxPolicy(checkers_environment_, estimator, players_[0], max_depth_));
}


Move CheckersAI::action_selection(const Checkerboard checkerboard, const CheckersPlayer current_player) {
	std::shared_ptr<Player> player{ (current_player == std::any_cast<CheckersPlayer>(players_[0]->player)) ? players_[0] : players_[1] };
	const std::shared_ptr<State> state{ std::make_shared<State>(State{ checkerboard, 0.0, false, player }) };
	checkers_environment_->set_state(state);
	const std::shared_ptr<Action> action{ minimax_policy_->action_selection(state) };
	return std::any_cast<Move>(action->action);
}
