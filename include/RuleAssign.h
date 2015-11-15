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
        int value(StateTuple states);
        virtual ~RuleAssign();
    protected:
    private:
        string variable;
};

#endif // RULEASSIGN_H
