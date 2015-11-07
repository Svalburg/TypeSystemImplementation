#ifndef RULEFUNCDEF_H
#define RULEFUNCDEF_H

#include <Rule.h>


class RuleFuncDef : public Rule
{
    public:
        RuleFuncDef(string statement, Rule* left, Rule* right);
        string toStringSigma(string states);
        string toStringE(string states);
        string toStringV(string states);
        virtual ~RuleFuncDef();
    protected:
    private:
};

#endif // RULEFUNCDEF_H
