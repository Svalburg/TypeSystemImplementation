#include "RuleConst.h"

RuleConst::RuleConst(string statement)
{
    this->statement = statement;
    ruleName = "Const";
}

string RuleConst::toStringSigma(string states)
{
    return states;
}

string RuleConst::toStringE(string states)
{
    return "td_ec(t_const)(" + states + ')';
}

string RuleConst::toStringV(string states)
{
    return "Const_N(" + statement + ")(" + states + ')';
}

RuleConst::~RuleConst()
{
    //dtor
}
