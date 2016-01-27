#ifndef RULEINPUT_H
#define RULEINPUT_H

#include <Rule.h>


class RuleInput : public Rule
{
    public:
        RuleInput(string statement);
        string toStringSigma();
        string toStringE();
        string toStringV();
        Value* value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
        virtual ~RuleInput();
    protected:
    private:
};

#endif // RULEINPUT_H
