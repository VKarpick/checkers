#include "ai/checkers_trainer.h"
#include "checkers_sfml.h"


int main()
{
    //double starting_weight{ 0.1 };
    //std::vector<double> weights{ 0 };
    //for (int i = 0; i < 37; ++i) {
    //    for (int j = 0; j < 2; ++j) {
    //        double current_weight{ (j == 0) ? starting_weight : -starting_weight };
    //        weights.push_back(current_weight);
    //        if (i < 32) {
    //            weights.push_back(current_weight);
    //        }
    //    }
    //}

    //unsigned int n_save_rounds{ 1 };
    //unsigned int n_episodes{ 10 };
    //for (unsigned int round_no = 0; round_no < n_save_rounds; ++round_no) {
    //    std::cout << "Round " << round_no << " of " << n_save_rounds << std::endl;
    //    CheckersTrainer checkers_trainer(weights, n_episodes, constants::default_max_depth, 0.0001);
    //    //CheckersTrainer checkers_trainer(n_episodes, constants::default_max_depth, 0.0001);
    //    checkers_trainer.train();
    //}

    //Checkers checkers;
    CheckersSFML checkers;
    checkers.start();

    return 0;
}
