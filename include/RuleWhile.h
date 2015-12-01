#ifndef RULEWHILE_H
#define RULEWHILE_H

#include "Rule.h"

class RuleWhile : public Rule
{
	public:
        RuleVar(string statement);
        string toStringSigma();
        string toStringE();
        string toStringV();
        int value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states);
		virtual ~RuleWhile() {}
	protected:
	private:
};

#endif // RULEWHILE_H