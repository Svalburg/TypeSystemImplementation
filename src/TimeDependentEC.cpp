#include "TimeDependentEC.h"

TimeDependentEC::TimeDependentEC(string componentName, Rule* rule)
{
	this->componentName = componentName;
	this->rule = rule;
}

int TimeDependentEC::getEnergyCost(StateTuple states)
{
    ValueInt* energyInt = dynamic_cast<ValueInt*>(rule->value(states));
    if(energyInt)
        return energyInt->getValue();
    else throw runtime_error("Exception: invalid phi function in component " + componentName + "\n");
}

string TimeDependentEC::getComponentName()
{
    return componentName;
}

TimeDependentEC::~TimeDependentEC()
{
	
}