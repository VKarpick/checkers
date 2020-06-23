/*-------------------------------------------------------------------------------------------------
 Trains the AI for Checkers using TD Leaf.

 It's best to initialize weights to something other than zero to avoid constantly forced draws 
 which will never update weights if they're set at 0.
-------------------------------------------------------------------------------------------------*/


#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "constants.h"


class CheckersTrainer {
public:
    CheckersTrainer(int n_episodes = 1, int max_depth = 1, double step_size = 0.01, bool is_printing_episodes = true,
        std::string read_filename = constants::weights_file, 
        std::string write_filename = constants::weights_file);
    CheckersTrainer(std::vector<double> weights, int n_episodes = 1, int max_depth = 1, double step_size = 0.01, 
        bool is_printing_episodes = true, std::string write_filename = constants::weights_file);
    
    
    static std::vector<double> read_weights(const std::string filename);
    void write_weights(const std::string filename, const std::vector<double> weights);
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
