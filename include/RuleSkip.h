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
        int value(StateTuple states);
		StateTuple sigma(StateTuple states);
        virtual ~RuleSkip();
    protected:
    private:
};

#endif // RULESKIP_H
