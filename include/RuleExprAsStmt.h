#ifndef RULEEXPRASSTMT_H
#define RULEEXPRASSTMT_H

#include <Rule.h>


class RuleExprAsStmt : public Rule
{
    public:
        RuleExprAsStmt(Rule* branch);
        string toStringSigma();
        string toStringE();
        string toStringV();
        Value* value(StateTuple states);
		StateTuple sigma(StateTuple states);
        int energy(StateTuple states, bool output = true);
        virtual ~RuleExprAsStmt();
    protected:
    private:
};

#endif // RULEEXPRASSTMT_H
