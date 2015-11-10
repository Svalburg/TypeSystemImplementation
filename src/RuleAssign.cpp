#include "RuleAssign.h"

RuleAssign::RuleAssign(string variable, Rule* branch)
{
    this->statement = variable + " := " + branch->getStatement();
    branches.push_back(branch);
    middle = 0;
    ruleName = "Assign";
}

string RuleAssign::toStringSigma(string states)
{
    size_t found = statement.find(":=");
    //extract variable name from statement
    string x;
    if(found != string::npos)
        x = statement.erase(found);
    else
        exit(-2);
    string sigma = branches.at(middle)->toStringSigma(states);
    string v = branches.at(middle)->toStringV(states);
    return "Assign_" + x + '(' + v + ")(" + sigma + ')';
}

string RuleAssign::toStringE(string states)
{
    string sigma = branches.at(middle)->toStringSigma(states);
    string e = branches.at(middle)->toStringE(states);
    return e + " + " + "td_ec(t_assign)(" + sigma + ')';
}

string RuleAssign::toStringV(string states)
{
    return branches.at(middle)->toStringV(states);
}

RuleAssign::~RuleAssign()
{
    //dtor
}
