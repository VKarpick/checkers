#include <iostream>

#include "ai/checkers_trainer.h"
#include "checkers.h"


int main()
{
    CheckersTrainer checkers_trainer(100, 6);
    //CheckersTrainer checkers_trainer(std::vector<double>(128, 2.0), 100, 4);
    //CheckersTrainer checkers_trainer(1, 1);
    checkers_trainer.train();
}
