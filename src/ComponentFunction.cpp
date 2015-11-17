#include "ComponentFunction.h"

ComponentFunction::ComponentFunction(string component, string name, string argumentName, 
int time, int energy, Rule* definition)
{
	this->component = component;
	this->name = name;
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

string ComponentFunction::getComponent()
{
	return component;
}

string ComponentFunction::getName()
{
	return name;
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

