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
        Value* value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
        virtual ~RuleIf();
    protected:
    private:
};

#endif // RULEIF_H
