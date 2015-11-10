#include "RuleConst.h"

RuleConst::RuleConst(string statement)
{
    this->statement = statement;
    ruleName = "Const";
}

string RuleConst::toStringSigma()
{
    return states;
}

string RuleConst::toStringE()
{
    return "td_ec(t_const)";
}

string RuleConst::toStringV()
{
    return "Const_N(" + statement + ")";
}

RuleConst::~RuleConst()
{
    //dtor
}
