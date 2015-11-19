#ifndef RULECALLCMPF_H
#define RULECALLCMPF_H

#include <Rule.h>


class RuleCallCmpF : public Rule
{
    public:
        RuleCallCmpF(string componentName, string functionName, Rule* branch);
        string toStringSigma();
        string toStringE();
        string toStringV();
        int value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states);
        virtual ~RuleCallCmpF();
    protected:
    private:
        string componentName;
        string functionName;
};

#endif // RULECALLCMPF_H
