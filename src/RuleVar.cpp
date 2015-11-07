#include "RuleVar.h"

RuleVar::RuleVar(string statement)
{
    this->statement = statement;
    ruleName = "Var";
}

string RuleVar::toStringSigma(string states)
{
    return states;
}

string RuleVar::toStringE(string states)
{
    return "td_ec(t_var)(" + states + ')';
}

string RuleVar::toStringV(string states)
{
    return "Lookup_" + statement + '(' + states + ')';
}

RuleVar::~RuleVar()
{
    //dtor
}
