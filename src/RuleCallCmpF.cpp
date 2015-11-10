#include "RuleCallCmpF.h"

RuleCallCmpF::RuleCallCmpF(string functionName, Rule* branch)
{
    this->functionName = functionName;
    this->statement = functionName + '(' + branch->getStatement() + ')';
    branches.push_back(branch);
    middle = 0;
    ruleName = "CallCmpF";
}

string RuleCallCmpF::toStringSigma()
{
    string sigma = branches.at(middle)->toStringSigma();
    return "Sigma_" + functionName + '(' + sigma + ')';
}

string RuleCallCmpF::toStringE()
{
    string v = branches.at(middle)->toStringV();
    string e = branches.at(middle)->toStringE();
    string sigma = branches.at(middle)->toStringSigma();
    return e + " + td_ec(t_" + functionName + ")(" + sigma + ") + E_" + functionName + "[" + v + "](" + sigma + ')';
}

string RuleCallCmpF::toStringV()
{
    string v = branches.at(middle)->toStringV();
    return "V_" + functionName + "[" + v + "]";
}

RuleCallCmpF::~RuleCallCmpF()
{
    //dtor
}
