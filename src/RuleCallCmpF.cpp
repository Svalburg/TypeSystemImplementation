#include "RuleCallCmpF.h"

RuleCallCmpF::RuleCallCmpF(string cmpfname, Rule* branch)
{
    this->statement = cmpfname + '(' + branch->getStatement() + ')';
    branches.push_back(branch);
    middle = 0;
    ruleName = "CallCmpF";
}

string RuleCallCmpF::toStringSigma(string states)
{
    string sigma = branches.at(middle)->toStringSigma(states);
    return "Sigma_" + functionName + '(' + sigma + ')';
}

string RuleCallCmpF::toStringE(string states)
{
    string v = branches.at(middle)->toStringV(states);
    string e = branches.at(middle)->toStringE(states);
    string sigma = branches.at(middle)->toStringSigma(states);
    return e + " + td_ec(t_" + functionName + ")(" + sigma + ") + E_" + functionName + "[" + v + "](" + sigma + ')';
}

string RuleCallCmpF::toStringV(string states)
{
    string v = branches.at(middle)->toStringV(states);
    return "V_" + functionName + "[" + v + "](" + states + ')';
}

RuleCallCmpF::~RuleCallCmpF()
{
    //dtor
}
