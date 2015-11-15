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
        int value(StateTuple states);
		StateTuple sigma(StateTuple states);
        virtual ~RuleConst();
    protected:
    private:
};

#endif // RULECONST_H
