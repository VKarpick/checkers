#include "checkers_trainer.h"



CheckersTrainer::CheckersTrainer(int nEpisodes, int maxDepth, double stepSize, bool isPrintingEpisodes,
    std::string readFilename, std::string writeFilename) {

    nEpisodes_ = nEpisodes;
    maxDepth_ = maxDepth;
    stepSize_ = stepSize;
    isPrintingEpisodes_ = isPrintingEpisodes;
    readFilename_ = readFilename;
    writeFilename_ = writeFilename;
}


CheckersTrainer::CheckersTrainer(std::vector<double> weights, int nEpisodes, int maxDepth, double stepSize, bool isPrintingEpisodes,
    std::string writeFilename) {

    nEpisodes_ = nEpisodes;
    maxDepth_ = maxDepth;
    stepSize_ = stepSize;
    isPrintingEpisodes_ = isPrintingEpisodes;
    weights_ = weights;
    writeFilename_ = writeFilename;
}


std::vector<double> CheckersTrainer::readWeights(std::string filename) {
    std::ifstream weightsFile;
    weightsFile.open(filename.c_str());

    if (weightsFile.is_open()) {
        std::vector<double> weights;

        double weight;
        while (weightsFile >> weight) {
            weights.push_back(weight);
        }
        weightsFile.close();

        return weights;
    }
    else {
        std::cout << "Read file not found.  Weights will be initialized to zero." << std::endl;
        std::cout << "Press any key to continue... ";
        std::cin.ignore();
        std::cout << std::endl;
        
        return {};
    }
}


void CheckersTrainer::writeWeights(std::string filename, std::vector<double> weights) {
    std::ofstream weightsFile;
    weightsFile.open(filename.c_str());
    for (double weight : weights) {
        weightsFile << weight << std::endl;
    }
    weightsFile.close();
}


void CheckersTrainer::train() {
    CheckersEnvironment checkersEnvironment;
    State* state{ checkersEnvironment.reset() };

    if (!readFilename_.empty()) weights_ = readWeights(readFilename_);
    
    unsigned int featureSize{ checkersEnvironment.featurize(state).size() };
    bool hasCorrectSize{ weights_.size() == featureSize };
    if (!weights_.empty() && !hasCorrectSize) {
        std::cout << "Weights from file not of right size.  Weights will be initialized to zero." << std::endl;
        std::cout << "Press any key to continue... ";
        std::cin.ignore();
        std::cout << std::endl;
    }

    if (!hasCorrectSize) weights_.assign(featureSize, 0);

    TDEstimator estimator(stepSize_, weights_, true);
    TDLeaf td_leaf(&checkersEnvironment, &estimator, checkersEnvironment.getPlayers()[0], maxDepth_);
    td_leaf.train(nEpisodes_, isPrintingEpisodes_);
    
    if (writeFilename_ != "") writeWeights(writeFilename_, estimator.getWeights());
}
