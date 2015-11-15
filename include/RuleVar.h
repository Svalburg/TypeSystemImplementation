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
        StateTuple value(StateTuple states);
        virtual ~RuleVar();
    protected:
    private:
};

#endif // RULEVAR_H
