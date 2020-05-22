#pragma once

#include <fstream>
#include <iostream>

#include "checkers_environment.h"
#include "../reinforcement_learning/estimator.h"
#include "../reinforcement_learning/td_learning.h"


class TrainCheckers {
public:
    TrainCheckers(int max_depth = 6, int n_episodes = 1, bool is_printing_episodes = true, 
        std::string read_filename = "", std::string write_filename="");
    std::vector<double> read_weights(std::string filename);
    void write_weights(std::string filename, std::vector<double> weights);
    void train();

private:
    int max_depth_;
    int n_episodes_;
    bool is_printing_episodes_;
    std::string read_filename_;
    std::string write_filename_;
};
