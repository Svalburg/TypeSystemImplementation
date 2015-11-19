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
        int value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states);
        virtual ~RuleExprConcat();
    protected:
    private:
};

#endif // RULEEXPRCONCAT_H
