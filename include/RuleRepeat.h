#ifndef RULEREPEAT_H
#define RULEREPEAT_H

#include <Rule.h>


class RuleRepeat : public Rule
{
    public:
        RuleRepeat(Rule* left, Rule* right);
        string toStringSigma(string states);
        string toStringE(string states);
        string toStringV(string states);
        virtual ~RuleRepeat();
    protected:
    private:
};

#endif // RULEREPEAT_H
