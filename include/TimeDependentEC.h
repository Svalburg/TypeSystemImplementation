#ifndef TIMEDEPENDENTEC_H
#define TIMEDEPENDENTEC_H

#include "Rule.h"

class TimeDependentEC {
	public:
		//this class is a data holder and an instance has to exist for every
		//component, the rule needs to be an expression, the output
		//of the rule will be the current time dependent energy cost
		//associated with the current component.
		TimeDependentEC(string componentName, Rule* rule);
		//calls on the current time dependent energy costs for the given
		//set of states.
		int getEnergyCost(StateTuple states);
        string getComponentName();
		virtual ~TimeDependentEC();
	private:
		string componentName;
		Rule* rule;
};

#endif // TIMEDEPENDENTEC_H