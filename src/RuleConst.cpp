#include "RuleConst.h"
#include "Environment.h"

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

int RuleConst::energy(StateTuple states, bool output)
{
    int time = env->getTConst();
    int total_energy = td_ec(time, states);
    if(output)
        cout << "Energy usage of " + statement + " is: " <<total_energy << endl; 
    return total_energy;
}

RuleConst::~RuleConst()
{
    //dtor
}
