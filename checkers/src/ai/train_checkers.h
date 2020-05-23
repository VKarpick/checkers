#pragma once

#include <fstream>
#include <iostream>

#include "checkers_environment.h"
#include "../reinforcement_learning/estimator.h"
#include "../reinforcement_learning/td_learning.h"


// how Checkers AI was actually trained
// automatically defaults to TD Leaf - if using with max depth < 6, have to initialize weights to something other than zero
// otherwise, will continually force draws and never update weights
class TrainCheckers {
public:
    TrainCheckers(int max_depth = 6, int n_episodes = 1, bool is_printing_episodes = true,
        std::string read_filename = "..\\checkers\\src\\ai\\weights.dat", 
        std::string write_filename = "..\\checkers\\src\\ai\\weights.dat");
    TrainCheckers(std::vector<double> weights, int max_depth = 6, int n_episodes = 1, bool is_printing_episodes = true,
        std::string write_filename = "..\\checkers\\src\\ai\\weights.dat");
    std::vector<double> read_weights(std::string filename);
    void write_weights(std::string filename, std::vector<double> weights);
    void train();

private:
    int max_depth_;
    int n_episodes_;
    bool is_printing_episodes_;
    std::string read_filename_ = "";
    std::string write_filename_;
    std::vector<double> weights_{};
};
