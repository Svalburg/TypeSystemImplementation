#ifndef RULECALLF_H
#define RULECALLF_H

#include <Rule.h>


class RuleCallF : public Rule
{
    public:
        RuleCallF(string functionname, Rule* branch);
        string toStringSigma();
        string toStringE();
        string toStringV();
        Value* value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
        virtual ~RuleCallF();
    protected:
    private:
        string functionName;
};

#endif // RULECALLF_H
