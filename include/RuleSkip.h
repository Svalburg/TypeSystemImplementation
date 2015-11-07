#ifndef RULESKIP_H
#define RULESKIP_H

#include <Rule.h>


class RuleSkip : public Rule
{
    public:
        RuleSkip();
        string toStringSigma(string states);
        string toStringE(string states);
        string toStringV(string states);
        virtual ~RuleSkip();
    protected:
    private:
};

#endif // RULESKIP_H
