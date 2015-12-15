#ifndef RULEASSIGN_H
#define RULEASSIGN_H

#include <Rule.h>


class RuleAssign : public Rule
{
    public:
        RuleAssign(string variable, Rule* branch, bool writeComponentState = false);
        string toStringSigma();
        string toStringE();
        string toStringV();
        int value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
        virtual ~RuleAssign();
    protected:
    private:
        string variable;
		bool writeComponentState;
};

#endif // RULEASSIGN_H
