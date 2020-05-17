#pragma once

#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

const int kMaxPlays{ 150 };
const int kStepDistance{ 1 }, kJumpDistance{ 2 };
const vector<int> kColumnSteps{ -1, 1 };
const vector<string> kDefaultBoard{
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


class CheckersEnv {
public:
	char Opponent(char player);
	char Opponent();
	vector<int> BoardToFeatures(vector<string> board_to_convert);
	vector<int> BoardToFeatures();
	vector<string> Reset();
	double GetReward(vector<string> current_board, int n_plays, char player);
	double GetReward();
	tuple<vector<string>, double, bool> Step(vector<string> action_board);
	vector<vector<string>> PieceMoves(vector<string> start_board, vector<int> row_steps, 
		int row, int column, int new_dist, int capture_distance);
	vector<vector<string>> actions(vector<string> start_board, char player);
	vector<vector<string>> actions();

private:
	vector<string> board_{ kDefaultBoard };
	char current_player_{ 'r' };
	int play_counter_{ 0 };

};
