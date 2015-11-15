#ifndef RULEFUNCDEF_H
#define RULEFUNCDEF_H

#include <Rule.h>


class RuleFuncDef : public Rule
{
    public:
        RuleFuncDef(string functionname, Rule* left, Rule* right);
        string toStringSigma();
        string toStringE();
        string toStringV();
        StateTuple value(StateTuple states);
        virtual ~RuleFuncDef();
    protected:
    private:
};

#endif // RULEFUNCDEF_H
