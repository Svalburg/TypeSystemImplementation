#include "ComponentFunction.h"

ComponentFunction::ComponentFunction(string component, string name, string argumentName, 
        Rule* definition, int energy, int time) : Function(name, argumentName, definition)
{
    this->component = component;
    this->time = time;
    this->energy = energy;
}

string ComponentFunction::getComponent()
{
    return component;
}

int ComponentFunction::getTime()
{
    return time;
}

int ComponentFunction::getEnergy()
{
    return energy;
}

ComponentFunction::~ComponentFunction()
{
}

