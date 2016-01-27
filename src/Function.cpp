#include "Function.h"

Function::Function(string name, string argumentName, 
Rule* definition)
{
	this->name = name;
    this->argumentName = argumentName;
    this->definition = definition;
}

StateTuple Function::sigma(StateTuple states)
{
    return definition->sigma(states);
}

Value* Function::value(StateTuple states)
{
    return definition->value(states);
}

int Function::energy(StateTuple states, bool output)
{
	return definition->energy(states, output);
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

