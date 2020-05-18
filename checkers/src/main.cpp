#include <iostream>
#include "reinforcement_learning/policy.h"

int main()
{
    std::any a = 0;
    std::any b = 1;
    std::cout << (std::any_cast<int>(a) == 0) << std::endl;

}
