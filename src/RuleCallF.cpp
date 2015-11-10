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
    string sigma = branches.at(middle)->toStringSigma();
    string path = branches.at(middle)->getPath();
    return "Split( " + sigma + ", [ x_" + functionName + "<- V_" + path + ", Sigma_" + path +
            "] >>> Sigma_" + functionName;
}

string RuleCallF::toStringE()
{
    string path = branches.at(middle)->getPath();
    string e = branches.at(middle)->toStringE();
    string sigma = branches.at(middle)->toStringSigma();
    return e + " + ( [ x_" + functionName + "<- V_" + path + ", Sigma_" + path +
            "] >>> E_" + functionName + " )" ;
}

string RuleCallF::toStringV()
{
    string path = branches.at(middle)->getPath();
    return "[ x_" + functionName + "<- V_" + path + ", Sigma_" + path + "] >>> V_" + functionName;
}

RuleCallF::~RuleCallF()
{
    //dtor
}
