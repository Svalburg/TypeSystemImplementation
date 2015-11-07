#ifndef RULEBINOP_H
#define RULEBINOP_H

#include <Rule.h>


class RuleBinOp : public Rule
{
    public:
        RuleBinOp(string statement, Rule* left, Rule* right);
        string toStringSigma(string states);
        string toStringE(string states);
        string toStringV(string states);
        virtual ~RuleBinOp();
    protected:
    private:
        string binop;
};

#endif // RULEBINOP_H
