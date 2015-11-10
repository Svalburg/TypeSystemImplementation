#ifndef RULESTMTCONCAT_H
#define RULESTMTCONCAT_H

#include <Rule.h>


class RuleStmtConcat : public Rule
{
    public:
        RuleStmtConcat(Rule* left, Rule* right);
        string toStringSigma(string states);
        string toStringE(string states);
        string toStringV(string states);
        virtual ~RuleStmtConcat();
    protected:
    private:
};

#endif // RULESTMTCONCAT_H
