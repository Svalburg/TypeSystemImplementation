#include "RuleAssign.h"

RuleAssign::RuleAssign(string variable, Rule* branch)
{
    this->variable = variable;
    this->statement = variable + " := " + branch->getStatement();
    branches.push_back(branch);
    middle = 0;
    ruleName = "Assign";
}

string RuleAssign::toStringSigma()
{
    string sigma = branches.at(middle)->toStringSigma();
    string v = branches.at(middle)->toStringV();
    return sigma + " >>> Assign_" + variable + '(' + v + ")";
}

string RuleAssign::toStringE()
{
    string sigma = branches.at(middle)->toStringSigma();
    string e = branches.at(middle)->toStringE();
    return e + " + (" + sigma + " >>> td_ec(t_assign) )";
}

string RuleAssign::toStringV()
{
    return branches.at(middle)->toStringV();
}

int RuleAssign::value(StateTuple states)
{
    return branches.at(middle)->value(states);
}

StateTuple RuleAssign::sigma(StateTuple states)
{
    StateTuple sigma1 = branches.at(middle)->sigma(states);
    sigma1.declarePState(variable, this->value(states));
    return sigma1;
}

int RuleAssign:energy(StateTuple states)
{
    int e = branches.at(middle)->energy(states);
    StateTuple sigma = branches.at(middle)->sigma(states);
    int time = env.getTAssign();
    return e + td_ec(time, sigma);
}

RuleAssign::~RuleAssign()
{
    //dtor
}
