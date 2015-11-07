#ifndef RULEASSIGN_H
#define RULEASSIGN_H

#include <Rule.h>


class RuleAssign : public Rule
{
    public:
        RuleAssign(string statement, Rule* branch);
        string toStringSigma(string states);
        string toStringE(string states);
        string toStringV(string states);
        virtual ~RuleAssign();
    protected:
    private:
};

#endif // RULEASSIGN_H
