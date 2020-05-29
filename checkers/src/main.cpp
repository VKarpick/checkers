#include <iostream>

#include "ai/checkers_trainer.h"
#include "checkers.h"


int main()
{
    CheckersTrainer checkersTrainer(100, 6);
    //CheckersTrainer checkersTrainer(std::vector<double>(128, 2.0), 100, 2);
    //CheckersTrainer checkersTrainer(1, 1);
    checkersTrainer.train();

    //{
    //    std::shared_ptr<Node<int>> node1{ std::make_shared<Node<int>>(1) };
    //    node1->addChild(std::make_shared<Node<int>>(2));
    //    //Minimax<int> minimax([](std::shared_ptr<Node<int>>) { return 1.0; }, 2, [](std::shared_ptr<Node<int>>) {});
    //    //node1 = std::make_shared<Node<int>>(2);
    //    //node1->removeChild(node1->getChildren()[0]);
    //    node1 = nullptr;
    //}
        
    /*Checkers checkers;
    checkers.play();*/
    /*checkers.executeInputMove(0);
    checkers.printMoveBoards();*/
    /*checkers.boardTry();
    checkers.printMoveBoards();*/
}
