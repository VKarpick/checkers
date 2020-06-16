#include <iostream>

#include <SFML/Graphics.hpp>

#include "ai/checkers_trainer.h"
#include "checkers.h"
#include "checkers_sfml.h"



int main()
{
    //CheckersTrainer checkers_trainer(50, 6, 0.1);
    //CheckersTrainer checkers_trainer(std::vector<double>(128, 2.0), 100, 4);
    //CheckersTrainer checkers_trainer(1, 1);
    //checkers_trainer.train();

    //Checkers checkers;
    CheckersSFML checkers;
    checkers.start();

    return 0;
}
