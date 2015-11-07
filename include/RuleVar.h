#ifndef RULEVAR_H
#define RULEVAR_H

#include "Rule.h"

class RuleVar : public Rule
{
    public:
        RuleVar(string statement);
        string toStringSigma(string states);
        string toStringE(string states);
        string toStringV(string states);
        virtual ~RuleVar();
    protected:
    private:
};

#endif // RULEVAR_H
