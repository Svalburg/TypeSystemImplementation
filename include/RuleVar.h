#ifndef RULEVAR_H
#define RULEVAR_H

#include "Rule.h"

class RuleVar : public Rule
{
    public:
        RuleVar(string statement, bool readComponentState = false);
        string toStringSigma();
        string toStringE();
        string toStringV();
        int value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states);
        virtual ~RuleVar();
    protected:
	private:
	bool readComponentState;
};

#endif // RULEVAR_H
