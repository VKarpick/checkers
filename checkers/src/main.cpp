#include <iostream>

#include "ai/checkers_trainer.h"
#include "checkers.h"


int main()
{
    CheckersTrainer checkersTrainer(std::vector<double>(128, 1.0), 3, 1);
    checkersTrainer.train();

    /*Checkers checkers;
    checkers.play();*/
    /*checkers.executeInputMove(0);
    checkers.printMoveBoards();*/
    /*checkers.boardTry();
    checkers.printMoveBoards();*/
}
