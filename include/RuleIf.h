#ifndef RULEIF_H
#define RULEIF_H

#include <Rule.h>


class RuleIf : public Rule
{
    public:
        RuleIf(string statement, Rule* left, Rule* middle, Rule* right);
        string toStringSigma(string states);
        string toStringE(string states);
        string toStringV(string states);
        virtual ~RuleIf();
    protected:
    private:
};

#endif // RULEIF_H
