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
    TrainCheckers(int nEpisodes = 1, int maxDepth = 6, bool isPrintingEpisodes = true,
        std::string readFilename = "..\\checkers\\src\\ai\\weights.dat", 
        std::string writeFilename = "..\\checkers\\src\\ai\\weights.dat");
    TrainCheckers(std::vector<double> weights, int nEpisodes = 1, int maxDepth = 6, bool isPrintingEpisodes = true,
        std::string writeFilename = "..\\checkers\\src\\ai\\weights.dat");
    
    
    std::vector<double> readWeights(std::string filename);
    void writeWeights(std::string filename, std::vector<double> weights);
    void train();

private:
    int maxDepth_;
    int nEpisodes_;
    bool isPrintingEpisodes_;
    std::string readFilename_ = "";
    std::string writeFilename_;
    std::vector<double> weights_{};
};
