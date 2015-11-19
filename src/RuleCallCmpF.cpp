#include "RuleCallCmpF.h"
#include "Environment.h"

RuleCallCmpF::RuleCallCmpF(string componentName, string functionName, Rule* branch)
{
    this->componentName = componentName;
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
    int v_ex = branches.at(middle)->value(states);
    StateTuple sigma_ex = branches.at(middle)->sigma(states);
    ComponentFunction* function = env->getComponentFunction(componentName, functionName);
    sigma_ex.declarePState(function->getArgumentName(), v_ex);
    return function->value(sigma_ex);    
}

StateTuple RuleCallCmpF::sigma(StateTuple states)
{
    int v_ex = branches.at(middle)->value(states);
    StateTuple sigma_ex = branches.at(middle)->sigma(states);
    vector<StateTuple::declaration> pstate = sigma_ex.getPState();
    ComponentFunction* function = env->getComponentFunction(componentName, functionName);
    sigma_ex.declarePState(function->getArgumentName(), v_ex);
    StateTuple sigma_f = function->sigma(sigma_ex);
    vector<StateTuple::declaration> cstate = sigma_f.getCState();
    return *(new StateTuple(pstate, cstate));
}

RuleCallCmpF::~RuleCallCmpF()
{
    //dtor
}
