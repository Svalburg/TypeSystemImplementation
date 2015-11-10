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
        virtual ~RuleSkip();
    protected:
    private:
};

#endif // RULESKIP_H
