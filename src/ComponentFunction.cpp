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
    ValueInt* energyInt = dynamic_cast<ValueInt*>(energyFunc->value(states));
    if(energyInt)
        return energyInt->getValue();
    else throw runtime_error("Exception: invalide energy function in component function: " + component + "." + name + "\n");
}

ComponentFunction::~ComponentFunction()
{
}

