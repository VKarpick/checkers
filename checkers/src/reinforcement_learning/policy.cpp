#include "policy.h"

Policy::Policy(Environment* env) { env_ = env; }


int RandomWalkPolicy::action_selection(std::vector<int> actions) {
	random_shuffle(actions.begin(), actions.end());
	return actions[0];
}

int MinimaxPolicy::action_selection(std::vector<int> actions) {
	return 0;
}

void MinimaxPolicy::reset_node() {

}
