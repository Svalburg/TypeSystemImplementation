#ifndef RULEASSIGN_H
#define RULEASSIGN_H

#include <Rule.h>


class RuleAssign : public Rule
{
    public:
        RuleAssign(string variable, Rule* branch);
        string toStringSigma();
        string toStringE();
        string toStringV();
        Value* value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
        virtual ~RuleAssign();
    protected:
    private:
        string variable;
};

#endif // RULEASSIGN_H
