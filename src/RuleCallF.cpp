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

string RuleCallF::toStringSigma()
{
    string v = branches.at(middle)->toStringV();
    string sigma = branches.at(middle)->toStringSigma();
    return "Sigma_" + functionName + '[' + v + "](" + sigma + ')';
}

string RuleCallF::toStringE()
{
    string v = branches.at(middle)->toStringV();
    string e = branches.at(middle)->toStringE();
    string sigma = branches.at(middle)->toStringSigma();
    return e + " + E_" + functionName + "[" + v + "](" + sigma + ')';
}

string RuleCallF::toStringV()
{
    string v = branches.at(middle)->toStringV();
    return "V_" + functionName + "[" + v + "]";
}

RuleCallF::~RuleCallF()
{
    //dtor
}
