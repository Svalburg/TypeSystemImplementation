#include "RuleCallF.h"

RuleCallF::RuleCallF(string functionname, Rule* branch)
{
    this->statement = functionName + '(' + branch->getStatement() + ')';
    branches.push_back(branch);
    middle = 0;
    ruleName = "CallF";
    //retrieve the function name from the statement
    size_t found = statement.find("(");
    if(found != string::npos)
        functionName = statement.erase(found);
    else
        exit(-1);
}

string RuleCallF::toStringSigma(string states)
{
    string v = branches.at(middle)->toStringV(states);
    string sigma = branches.at(middle)->toStringSigma(states);
    return "Sigma_" + functionName + '[' + v + "](" + sigma + ')';
}

string RuleCallF::toStringE(string states)
{
    string v = branches.at(middle)->toStringV(states);
    string e = branches.at(middle)->toStringE(states);
    string sigma = branches.at(middle)->toStringSigma(states);
    return e + " + E_" + functionName + "[" + v + "](" + sigma + ')';
}

string RuleCallF::toStringV(string states)
{
    string v = branches.at(middle)->toStringV(states);
    return "V_" + functionName + "[" + v + "](" + states + ')';
}

RuleCallF::~RuleCallF()
{
    //dtor
}
