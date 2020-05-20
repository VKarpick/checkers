#include <iostream>

#include "ai/checkers_environment.h"


int main()
{
    CheckersEnvironment env;
    State* state = env.reset();
    /*for (auto row : std::any_cast<std::vector<std::string>>(state->observation)) {
        std::cout << row << std::endl;
    }*/
    /*std::vector<double> features{ env.featurize(state) };
    for (double feature : features) {
        std::cout << feature << std::endl;
    }*/

    std::cout << env.actions().size() << std::endl;
}
