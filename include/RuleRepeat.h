#ifndef RULEREPEAT_H
#define RULEREPEAT_H

#include <Rule.h>


class RuleRepeat : public Rule
{
    public:
        RuleRepeat(Rule* left, Rule* right);
        string toStringSigma();
        string toStringE();
        string toStringV();
        int value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
        virtual ~RuleRepeat();
    protected:
    private:
};

#endif // RULEREPEAT_H
