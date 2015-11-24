#include "ComponentFunction.h"

ComponentFunction::ComponentFunction(string component, string name, string argumentName, 
        Rule* definition, Rule* energyFunc, int time) : Function(name, argumentName, definition)
{
    this->component = component;
    this->time = time;
    this->energyFunc = energyFunc;
}

string ComponentFunction::getComponent()
{
    return component;
}

int ComponentFunction::getTime()
{
    return time;
}

int ComponentFunction::energy(StateTuple states)
{
    return energyFunc->value(states);
}

ComponentFunction::~ComponentFunction()
{
}

