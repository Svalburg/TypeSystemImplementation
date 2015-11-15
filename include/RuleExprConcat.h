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
        StateTuple value(StateTuple states);
        virtual ~RuleExprConcat();
    protected:
    private:
};

#endif // RULEEXPRCONCAT_H
