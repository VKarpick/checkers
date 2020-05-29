#include <iostream>

#include "ai/checkers_trainer.h"
#include "checkers.h"


int main()
{
    //CheckersTrainer checkersTrainer(100, 6);
    CheckersTrainer checkersTrainer(std::vector<double>(128, 2.0), 3, 4);
    checkersTrainer.train();

    /*Checkers checkers;
    checkers.play();*/
    /*checkers.executeInputMove(0);
    checkers.printMoveBoards();*/
    /*checkers.boardTry();
    checkers.printMoveBoards();*/
}
