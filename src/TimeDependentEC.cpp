#include "TimeDependentEC.h"

TimeDependentEC::TimeDependentEC(string componentName, Rule* rule)
{
	this->componentName = componentName;
	this->rule = rule;
}

int TimeDependentEC::getEnergyCost(StateTuple states)
{
	return rule->value(states);
}

string TimeDependentEC::getComponentName()
{
    return componentName;
}

TimeDependentEC::~TimeDependentEC()
{
	
}