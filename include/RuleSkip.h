#ifndef RULESKIP_H
#define RULESKIP_H

#include <Rule.h>


class RuleSkip : public Rule
{
    public:
        RuleSkip();
        string toStringSigma();
        string toStringE();
        string toStringV();
        Value* value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
        virtual ~RuleSkip();
    protected:
    private:
};

#endif // RULESKIP_H
