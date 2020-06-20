/*-------------------------------------------------------------------------------------------------
 Trains the AI for Checkers using TD Leaf.

 It's best to initialize weights to something other than zero to avoid constantly forced draws 
 which will never update weights if they're set at 0.
-------------------------------------------------------------------------------------------------*/


#pragma once

#include <fstream>
#include <iostream>

#include "checkers_environment.h"
#include "../reinforcement_learning/estimator.h"
#include "../reinforcement_learning/td_learning.h"


class CheckersTrainer {
public:
    CheckersTrainer(int n_episodes = 1, int max_depth = 1, double step_size = 0.01, bool is_printing_episodes = true,
        std::string read_filename = "..\\checkers\\data\\weights.dat", 
        std::string write_filename = "..\\checkers\\data\\weights.dat");
    CheckersTrainer(std::vector<double> weights, int n_episodes = 1, int max_depth = 1, double step_size = 0.01, 
        bool is_printing_episodes = true, std::string write_filename = "..\\checkers\\data\\weights.dat");
    
    
    static std::vector<double> read_weights(std::string filename);
    void write_weights(std::string filename, std::vector<double> weights);
    void train();


private:
    int n_episodes_{ 1 };
    const int max_depth_;
    double step_size_{ 0.01 };
    bool is_printing_episodes_{ true };
    std::string read_filename_{ "" };
    std::string write_filename_{ "" };
    std::vector<double> weights_{};
};
