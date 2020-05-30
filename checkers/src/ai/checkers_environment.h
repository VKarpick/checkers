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
	std::vector<std::shared_ptr<Player>> get_players();

	
	std::shared_ptr<State> reset() override;
	std::shared_ptr<State> step(std::shared_ptr<Action> action) override;
	std::shared_ptr<State> step(std::shared_ptr<State> state, std::shared_ptr<Action> action) override;
	std::vector<std::shared_ptr<Action>> get_actions() override;
	std::vector<std::shared_ptr<Action>> get_actions(std::shared_ptr<State> state) override;
	std::vector<double> featurize(std::shared_ptr<State> state) override;
	std::vector<std::string> convert_state_to_board(std::shared_ptr<State> state);
	std::shared_ptr<Player> opponent(std::shared_ptr<Player> current_player);


private:
	std::shared_ptr<State> state_{ nullptr };
	int nPlays_{ 0 };
	const std::vector<std::shared_ptr<Player>> players_{ std::make_shared<Player>(Player{'r'}), std::make_shared<Player>(Player{'w'}) };
};
