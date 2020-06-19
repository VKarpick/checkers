#include <iostream>

#include <SFML/Graphics.hpp>

#include "ai/checkers_trainer.h"
#include "checkers.h"
#include "checkers_sfml.h"

#include <ctime>

int main()
{
    for (int save_round = 0; save_round < 50; ++save_round) {
        std::cout << "Round " << save_round << std::endl;
        CheckersTrainer checkers_trainer(std::vector<double>(139, 1.0), 10, constants::default_max_depth, 0.0001);
        checkers_trainer.train();
    }

    //Checkers checkers;
    /*CheckersSFML checkers;
    checkers.start();*/

    return 0;
}
