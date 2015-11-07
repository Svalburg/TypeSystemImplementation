#include "RuleCallCmpF.h"

RuleCallCmpF::RuleCallCmpF(string statement, Rule* branch)
{
    this->statement = statement;
    branches.push_back(branch);
    middle = 0;
    ruleName = "CallCmpF";
    //retrieve the function name from the statement
    size_t found = statement.find("(");
    if(found != string::npos)
        functionName = statement.erase(found);
    else
        exit(-1);
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
