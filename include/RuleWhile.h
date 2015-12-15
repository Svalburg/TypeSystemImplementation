#ifndef RULEWHILE_H
#define RULEWHILE_H

#include "Rule.h"

class RuleWhile : public Rule
{
	public:
        RuleWhile(Rule* left, Rule* right);
        string toStringSigma();
        string toStringE();
        string toStringV();
        int value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
		virtual ~RuleWhile();
	protected:
	private:
};

#endif // RULEWHILE_H