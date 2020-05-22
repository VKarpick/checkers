#include <iostream>

#include "ai/train_checkers.h"


//TODO does policy need environment if td learning has it?
int main()
{
    TrainCheckers training(6, 1, true, "weights.dat", "weights.dat");
    training.train();
}
