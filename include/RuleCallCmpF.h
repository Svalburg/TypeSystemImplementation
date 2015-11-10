#ifndef RULECALLCMPF_H
#define RULECALLCMPF_H

#include <Rule.h>


class RuleCallCmpF : public Rule
{
    public:
        RuleCallCmpF(string cmpfname, Rule* branch);
        string toStringSigma(string states);
        string toStringE(string states);
        string toStringV(string states);
        virtual ~RuleCallCmpF();
    protected:
    private:
        string functionName;
};

#endif // RULECALLCMPF_H
