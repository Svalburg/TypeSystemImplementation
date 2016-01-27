#ifndef RULEEXPRCONCAT_H
#define RULEEXPRCONCAT_H

#include <Rule.h>


class RuleExprConcat : public Rule
{
    public:
        RuleExprConcat(Rule* left, Rule* right);
        string toStringSigma();
        string toStringE();
        string toStringV();
        Value* value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
        virtual ~RuleExprConcat();
    protected:
    private:
};

#endif // RULEEXPRCONCAT_H
