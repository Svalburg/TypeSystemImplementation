#ifndef RULEEXPRCONCAT_H
#define RULEEXPRCONCAT_H

#include <Rule.h>


class RuleExprConcat : public Rule
{
    public:
        RuleExprConcat(Rule* left, Rule* right);
        string toStringSigma(string states);
        string toStringE(string states);
        string toStringV(string states);
        virtual ~RuleExprConcat();
    protected:
    private:
};

#endif // RULEEXPRCONCAT_H
