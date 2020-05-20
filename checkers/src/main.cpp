#include <iostream>

#include "ai/checkers_environment.h"
#include "reinforcement_learning/estimator.h"
#include "reinforcement_learning/td_learning.h"


//TODO does policy need environment if td learning has it?
int main()
{
    CheckersEnvironment env;
    State* state = env.reset();

    TDEstimator estimator(0.01, 128, true);
    RandomWalkPolicy policy(&env);

//    TDLambda td_lambda(&env, &estimator, &policy);
//    td_lambda.train(1, true);
    TDLeaf td_leaf(&env, &estimator, env.players()[0], 3);
    td_leaf.train(1, true);

    std::cout << "Weights" << std::endl;
    for (auto weight : estimator.weights()) {
        std::cout << weight << std::endl;
    }
}
