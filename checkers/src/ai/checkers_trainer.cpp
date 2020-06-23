#include "ai/checkers_trainer.h"

#include "ai/checkers_environment.h"
#include "reinforcement_learning/estimator.h"
#include "reinforcement_learning/td_learning.h"


CheckersTrainer::CheckersTrainer(int n_episodes, int max_depth, double step_size, bool is_printing_episodes,
    std::string read_filename, std::string write_filename) :
    n_episodes_(n_episodes),
    max_depth_(max_depth),
    step_size_(step_size),
    is_printing_episodes_(is_printing_episodes),
    read_filename_(read_filename),
    write_filename_(write_filename)
{}


CheckersTrainer::CheckersTrainer(std::vector<double> weights, int n_episodes, int max_depth, double step_size, bool is_printing_episodes,
    std::string write_filename) :
    n_episodes_(n_episodes),
    max_depth_(max_depth),
    step_size_(step_size),
    is_printing_episodes_(is_printing_episodes),
    weights_(weights),
    write_filename_(write_filename)
{}


std::vector<double> CheckersTrainer::read_weights(const std::string filename) {
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


void CheckersTrainer::write_weights(const std::string filename, const std::vector<double> weights) {
    std::ofstream weights_file;
    weights_file.open(filename.c_str());
    for (double weight : weights) {
        weights_file << weight << std::endl;
    }
    weights_file.close();
}


void CheckersTrainer::train() {
    std::shared_ptr<CheckersEnvironment> checkers_environment{ std::make_shared<CheckersEnvironment>() };
    std::shared_ptr<State> state{ checkers_environment->reset() };

    if (!read_filename_.empty()) {
        weights_ = read_weights(read_filename_);
    }

    const unsigned int feature_size{ checkers_environment->featurize(state).size() };
    const bool has_correct_size{ weights_.size() == feature_size };
    if (!has_correct_size) {
        if (!weights_.empty()) {
            std::cout << "Weights from file not of right size.  Weights will be initialized to zero." << std::endl;
            std::cout << "Press any key to continue... ";
            std::cin.ignore();
            std::cout << std::endl;
        }

        weights_.assign(feature_size, 0.0);
    }

    std::shared_ptr<TDEstimator> estimator{ std::make_shared<TDEstimator>(step_size_, weights_, true) };

    // use first player as max player
    std::shared_ptr<Player> max_player{ std::make_shared<Player>(Player{ checkers_environment->get_players()[0] }) };

    TDLeaf td_leaf(checkers_environment, estimator, max_player, max_depth_);
    
    td_leaf.train(n_episodes_, is_printing_episodes_);
    
    if (write_filename_ != "") write_weights(write_filename_, estimator->get_weights());
}
