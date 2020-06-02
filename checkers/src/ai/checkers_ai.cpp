#include "checkers_ai.h"



Move CheckersAI::action_selection() {
	std::shared_ptr<Action> action{ minimax_policy_->action_selection(state_) };
	return std::any_cast<Move>(action->action);
}


void CheckersAI::process_move(Move move) {
	std::shared_ptr<Action> action{ move };
	checkers_environment_.step(action);
}
