#ifndef RULEBINOP_H
#define RULEBINOP_H

#include <Rule.h>


class RuleBinOp : public Rule
{
    public:
        RuleBinOp(string binop, Rule* left, Rule* right);
        string toStringSigma();
        string toStringE();
        string toStringV();
        StateTuple value(StateTuple states);
        virtual ~RuleBinOp();
    protected:
    private:
        string binop;
};

#endif // RULEBINOP_H
