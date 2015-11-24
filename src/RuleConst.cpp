#include "RuleConst.h"

RuleConst::RuleConst(string statement)
{
    this->statement = statement;
    ruleName = "Const";
}

string RuleConst::toStringSigma()
{
    return "id";
}

string RuleConst::toStringE()
{
    return "td_ec(t_const)";
}

string RuleConst::toStringV()
{
    return "Const_N(" + statement + ")";
}

int RuleConst::value(StateTuple states)
{
    //returns the value of the constant as in the statement
    return atoi(statement.c_str());
}

StateTuple RuleConst::sigma(StateTuple states)
{
    return states;
}

int RuleConst::energy(StateTuple states)
{
    int time = env.getTConst();
    return td_ec(time, states);
}

RuleConst::~RuleConst()
{
    //dtor
}
