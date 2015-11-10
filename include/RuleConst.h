#ifndef RULECONST_H
#define RULECONST_H

#include <Rule.h>


class RuleConst : public Rule
{
    public:
        RuleConst(string statement);
        string toStringSigma();
        string toStringE();
        string toStringV();
        virtual ~RuleConst();
    protected:
    private:
};

#endif // RULECONST_H
