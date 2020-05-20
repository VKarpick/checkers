#pragma once

#include <string>

#include "../reinforcement_learning/environment.h"

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

private:
	int play_counter_{ 0 };
	const std::vector<Player*> players_{ new Player{'r'}, new Player{'w'} };
	int current_player_index_{ 0 };
};

//const int kMaxPlays{ 150 };
//const int kStepDistance{ 1 }, kJumpDistance{ 2 };
//const vector<int> kColumnSteps{ -1, 1 };
//
//class CheckersEnv {
//public:
//	char Opponent(char player);
//	char Opponent();
//	vector<int> BoardToFeatures(vector<string> board_to_convert);
//	vector<int> BoardToFeatures();
//	vector<string> Reset();
//	double GetReward(vector<string> current_board, int n_plays, char player);
//	double GetReward();
//	tuple<vector<string>, double, bool> Step(vector<string> action_board);
//	vector<vector<string>> PieceMoves(vector<string> start_board, vector<int> row_steps, 
//		int row, int column, int new_dist, int capture_distance);
//	vector<vector<string>> actions(vector<string> start_board, char player);
//	vector<vector<string>> actions();
