#include <iostream>

#include <SFML/Graphics.hpp>

#include "ai/checkers_trainer.h"
#include "checkers.h"
#include "checkers_sfml.h"

#include <ctime>

int main()
{
    //CheckersTrainer checkers_trainer(6, 5, 0.1);
    CheckersTrainer checkers_trainer(std::vector<double>(139, 1.0), 5, 5, 0.1);
    checkers_trainer.train();

    //Checkers checkers;
    //CheckersSFML checkers;
    //checkers.start();

    return 0;
}
