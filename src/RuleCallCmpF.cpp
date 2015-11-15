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
    string path = branches.at(middle)->getPath();
    return "Split( " + sigma + ", [ x_" + functionName + " <- V_" + path + ", Sigma_" + path +
            " ] >>> Sigma_" + functionName + " )";
}

string RuleCallCmpF::toStringE()
{
    string path = branches.at(middle)->getPath();
    string e = branches.at(middle)->toStringE();
    string sigma = branches.at(middle)->toStringSigma();
    return e + " + ( [ x_" + functionName + " <- V_" + path + ", Sigma_" + path +
            " ] >>> ( td_ec(t_" + functionName + ") + E_" + functionName + " ) )" ;
}

string RuleCallCmpF::toStringV()
{
    string path = branches.at(middle)->getPath();
    return "[ x_" + functionName + "<- V_" + path + ", Sigma_" + path + "] >>> V_" + functionName;
}

int RuleCallCmpF::value(StateTuple states)
{
    //not final version, awaiting implementation of an environment so Component function changes 
    //can be taken into account.
    return branches.at(middle)->value(states);
}

StateTuple RuleCallCmpF::sigma(StateTuple states)
{
    //not final version, awaiting implementation of an environment so Component function changes 
    //can be taken into account.
    StateTuple sigma1 = branches.at(middle)->sigma(states);
    vector<StateTuple::declaration> pstate = sigma1.getPState();
    vector<StateTuple::declaration> cstate = sigma1.getCState();
    StateTuple* newstate = new StateTuple(pstate, cstate);
    return (*newstate);
}

RuleCallCmpF::~RuleCallCmpF()
{
    //dtor
}
