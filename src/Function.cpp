#include "Function.h"

Function::ComponentFunction(string name, string argumentName, 
int energy, Rule* definition)
{
	this->name = name;
    this->argumentName = argumentName;
    this->energy = energy;
    this->definition = definition;
}

StateTuple Function::sigma(StateTuple states)
{
    return definition->sigma(states);
}

int Function::value(StateTuple states)
{
    return definition->value(states);
}

string Function::getArgumentName()
{
    return argumentName;
}

string Function::getName()
{
	return name;
}

Function::~Function()
{
    //dtor
}

