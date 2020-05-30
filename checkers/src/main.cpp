#include <iostream>

#include "ai/checkers_trainer.h"
#include "checkers.h"


int main()
{
    CheckersTrainer checkersTrainer(100, 6);
    //CheckersTrainer checkersTrainer(std::vector<double>(128, 2.0), 100, 2);
    //CheckersTrainer checkersTrainer(1, 1);
    checkersTrainer.train();
}
