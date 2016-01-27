#ifndef RULECONST_H
#define RULECONST_H

#include <Rule.h>

class RuleConst : public Rule
{
    public:
        RuleConst(string statement, Value* value);
        string toStringSigma();
        string toStringE();
        string toStringV();
        Value* value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
        virtual ~RuleConst();
    protected:
    private:
        Value* value;
};

#endif // RULECONST_H
