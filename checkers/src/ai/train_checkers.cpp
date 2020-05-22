#include "train_checkers.h"


TrainCheckers::TrainCheckers(int max_depth, int n_episodes, bool is_printing_episodes, 
    std::string read_filename, std::string write_filename) {

    max_depth_ = max_depth;
    n_episodes_ = n_episodes;
    is_printing_episodes_ = is_printing_episodes;
    read_filename_ = read_filename;
    write_filename_ = write_filename;
}

std::vector<double> TrainCheckers::read_weights(std::string filename) {
    filename = "..\\checkers\\src\\ai\\" + filename;
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
        return std::vector<double>(0);
    }
}

void TrainCheckers::write_weights(std::string filename, std::vector<double> weights) {
    std::ofstream weights_file;
    filename = "..\\checkers\\src\\ai\\" + filename;
    weights_file.open(filename.c_str());
    std::cout << (weights_file.is_open()) << std::endl;
    for (double weight : weights) {
        weights_file << weight << std::endl;
    }
    weights_file.close();
}

void TrainCheckers::train() {
    CheckersEnvironment env;
    State* state{ env.reset() };

    std::vector<double> weights{ read_weights(read_filename_) };
    unsigned int feature_size{ env.featurize(state).size() };
    bool has_correct_size{ weights.size() == feature_size };

    if (!has_correct_size && read_filename_ != "") {
        std::cout << "Weights from file not of right size." << std::endl;
        std::cout << "Continue without? (y/n) ";
        std::string continue_without;
        do {
            std::cin >> continue_without;
        } while (continue_without != "y" && continue_without != "Y" && continue_without != "n" && continue_without != "N");

        if (continue_without == "n" || continue_without == "N") return;
    }

    if (!has_correct_size) weights.assign(feature_size, 0);

    TDEstimator estimator(0.01, weights, true);
    TDLeaf td_leaf(&env, &estimator, env.players()[0], max_depth_);
    td_leaf.train(n_episodes_, is_printing_episodes_);
    
    if (write_filename_ != "") write_weights(write_filename_, estimator.weights());

    /*for (auto weight : weights) {
        std::cout << weight << std::endl;
    }*/
}
