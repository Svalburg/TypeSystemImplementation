#include "ComponentFunction.h"

ComponentFunction::ComponentFunction(string argumentName, int time, int energy, Rule* definition)
{
    this->argumentName = argumentName;
    this->time = time;
    this->energy = energy;
    this->definition = definition;
}

StateTuple ComponentFunction::sigma(StateTuple states)
{
    return definition->sigma(states);
}

int ComponentFunction::value(StateTuple states)
{
    return definition->value(states);
}

string ComponentFunction::getArgumentName()
{
    return argumentName;
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
    //dtor
}

