#include "TimeDependentEC.h"

TimeDependentEC::TimeDependentEC(string componentState, Rule* rule)
{
	this->componentState = componentState;
	this->rule = rule;
}

int TimeDependentEC::getEnergyCost(StateTuple states)
{
	return rule->value(states);
}

TimeDependentEC::~TimeDependentEC()
{
	
}