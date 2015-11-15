#ifndef RULEIF_H
#define RULEIF_H

#include <Rule.h>


class RuleIf : public Rule
{
    public:
        RuleIf(Rule* left, Rule* middle, Rule* right);
        string toStringSigma();
        string toStringE();
        string toStringV();
        int value(StateTuple states);
        virtual ~RuleIf();
    protected:
    private:
};

#endif // RULEIF_H
