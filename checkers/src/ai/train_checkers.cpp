#include "train_checkers.h"


TrainCheckers::TrainCheckers(int max_depth, int n_episodes, bool is_printing_episodes, 
    std::string read_filename, std::string write_filename) {

    max_depth_ = max_depth;
    n_episodes_ = n_episodes;
    is_printing_episodes_ = is_printing_episodes;
    read_filename_ = read_filename;
    write_filename_ = write_filename;
}

TrainCheckers::TrainCheckers(std::vector<double> weights, int max_depth, int n_episodes, bool is_printing_episodes,
    std::string write_filename) {

    max_depth_ = max_depth;
    n_episodes_ = n_episodes;
    is_printing_episodes_ = is_printing_episodes;
    weights_ = weights;
    write_filename_ = write_filename;
}

std::vector<double> TrainCheckers::read_weights(std::string filename) {
    std::ifstream weights_file;
    weights_file.open(filename.c_str());

    if (weights_file.is_open()) {
        std::vector<double> weights;

        double weight;
        while (weights_file >> weight) {
            weights.push_back(weight);
        }
        weights_file.close();

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

void TrainCheckers::write_weights(std::string filename, std::vector<double> weights) {
    std::ofstream weights_file;
    weights_file.open(filename.c_str());
    for (double weight : weights) {
        weights_file << weight << std::endl;
    }
    weights_file.close();
}

void TrainCheckers::train() {
    CheckersEnvironment env;
    State* state{ env.reset() };

    if (!read_filename_.empty()) weights_ = read_weights(read_filename_);
    
    unsigned int feature_size{ env.featurize(state).size() };
    bool has_correct_size{ weights_.size() == feature_size };
    if (!weights_.empty() && !has_correct_size) {
        std::cout << "Weights from file not of right size.  Weights will be initialized to zero." << std::endl;
        std::cout << "Press any key to continue... ";
        std::cin.ignore();
        std::cout << std::endl;
    }

    if (!has_correct_size) weights_.assign(feature_size, 0);

    TDEstimator estimator(0.01, weights_, true);
    TDLeaf td_leaf(&env, &estimator, env.players()[0], max_depth_);
    td_leaf.train(n_episodes_, is_printing_episodes_);
    
    if (write_filename_ != "") write_weights(write_filename_, estimator.weights());
}
