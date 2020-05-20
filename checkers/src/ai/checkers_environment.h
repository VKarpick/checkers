#pragma once

#include <string>

#include "../reinforcement_learning/environment.h"


const int kMaxPlays{ 150 };
const int kStepDistance{ 1 }, kJumpDistance{ 2 };
const std::vector<int> kColumnSteps{ -1, 1 };
const std::vector<std::string> kDefaultBoard{
	"            ",
	"            ",
	"   w w w w  ",
	"  w w w w   ",
	"   w w w w  ",
	"  - - - -   ",
	"   - - - -  ",
	"  r r r r   ",
	"   r r r r  ",
	"  r r r r   ",
	"            ",
	"            "
};


class CheckersEnvironment : public Environment {
public:
	State* reset() override;
	State* step(Action* action) override;
	State* step(State* state, Action* action) override;
	std::vector<Action*> actions() override;
	std::vector<Action*> actions(State* state) override;
	std::vector<double> featurize(State* state) override;
	std::vector<std::string> board_from_state(State* state);
	Player* opponent(Player* current_player);

private:
	State* state_;
	int play_counter_{ 0 };
	const std::vector<Player*> players_{ new Player{'r'}, new Player{'w'} };
	int current_player_index_{ 0 };
};
