#ifndef RULESTMTCONCAT_H
#define RULESTMTCONCAT_H

#include <Rule.h>


class RuleStmtConcat : public Rule
{
    public:
        RuleStmtConcat(Rule* left, Rule* right);
        string toStringSigma();
        string toStringE();
        string toStringV();
        Value* value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
        virtual ~RuleStmtConcat();
    protected:
    private:
};

#endif // RULESTMTCONCAT_H
