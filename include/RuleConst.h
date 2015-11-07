#ifndef RULECONST_H
#define RULECONST_H

#include <Rule.h>


class RuleConst : public Rule
{
    public:
        RuleConst(string statement);
        string toStringSigma(string states);
        string toStringE(string states);
        string toStringV(string states);
        virtual ~RuleConst();
    protected:
    private:
};

#endif // RULECONST_H
