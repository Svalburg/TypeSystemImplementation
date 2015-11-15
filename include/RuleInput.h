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
        int value(StateTuple states);
        virtual ~RuleInput();
    protected:
    private:
};

#endif // RULEINPUT_H
