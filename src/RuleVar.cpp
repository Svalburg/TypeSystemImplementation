#include "RuleVar.h"

RuleVar::RuleVar(string statement)
{
    this->statement = statement;
    ruleName = "Var";
}

string RuleVar::toStringSigma()
{
    return states;
}

string RuleVar::toStringE()
{
    return "td_ec(t_var)";
}

string RuleVar::toStringV()
{
    return "Lookup_" + statement + '(' + states + ')';
}

RuleVar::~RuleVar()
{
    //dtor
}
