#include "RuleCallF.h"
#include "Environment.h"

RuleCallF::RuleCallF(string functionName, Rule* branch)
{
    this->functionName = functionName;
    this->statement = functionName + '(' + branch->getStatement() + ')';
    branches.push_back(branch);
    middle = 0;
    ruleName = "CallF";
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

int RuleCallF::value(StateTuple states)
{
    int v_ex = branches.at(middle)->value(states);
    StateTuple sigma_ex = branches.at(middle)->sigma(states);
    Function* function = env->getFunction(functionName);
    sigma_ex.declarePState(function->getArgumentName(), v_ex);
    return function->value(sigma_ex);    
}

StateTuple RuleCallF::sigma(StateTuple states)
{
    int v_ex = branches.at(middle)->value(states);
    StateTuple sigma_ex = branches.at(middle)->sigma(states);
    vector<StateTuple::declaration> pstate = sigma_ex.getPState();
    Function* function = env->getFunction(functionName);
    sigma_ex.declarePState(function->getArgumentName(), v_ex);
    StateTuple sigma_f = function->sigma(sigma_ex);
    vector<StateTuple::declaration> cstate = sigma_f.getCState();
    return *(new StateTuple(pstate, cstate));
}

int RuleCallF::energy(StateTuple states, bool output)
{    
    int e_expr = branches.at(middle)->energy(states);
    int v_ex = branches.at(middle)->value(states);
    StateTuple sigma_ex = branches.at(middle)->sigma(states);
    Function* function = env->getFunction(functionName);
    sigma_ex.declarePState(function->getArgumentName(), v_ex);
    int e_func = function->energy(sigma_ex);
    return e_expr + e_func;
}

RuleCallF::~RuleCallF()
{
    //dtor
}
