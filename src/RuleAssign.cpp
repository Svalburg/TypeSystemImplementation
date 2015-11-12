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

RuleAssign::~RuleAssign()
{
    //dtor
}
