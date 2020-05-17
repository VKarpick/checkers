#pragma once

#include <vector>
#include <iostream>

struct State
{
	std::vector<double> observation;
	double reward;
	bool terminal;
	std::string info;
};

class Environment
{
public:
	virtual void reset(State* state) = 0;
	virtual void step() = 0;
};
