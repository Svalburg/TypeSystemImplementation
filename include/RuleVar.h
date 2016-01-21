#ifndef RULEVAR_H
#define RULEVAR_H

#include "Rule.h"

class RuleVar : public Rule
{
    public:
        RuleVar(string statement);
        string toStringSigma();
        string toStringE();
        string toStringV();
        int value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
        virtual ~RuleVar();
    protected:
	private:
};

#endif // RULEVAR_H
