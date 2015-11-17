#ifndef TIMEDEPENDENTEC_H
#define TIMEDEPENDENTEC_H

#include "Rule.h"

class TimeDependentEC {
	public:
		TimeDependentEC(string componentState, Rule* rule);
		int getEnergyCost(StateTuple states);
		virtual ~TimeDependentEC();
	private:
		string componentState;
		Rule* rule;
};

#endif // TIMEDEPENDENTEC_H