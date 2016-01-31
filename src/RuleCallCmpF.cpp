#include "RuleCallCmpF.h"
#include "Environment.h"

RuleCallCmpF::RuleCallCmpF(string componentName, string functionName, Rule* branch)
{
    this->componentName = componentName;
    this->functionName = functionName;
    this->statement = componentName + '.' + functionName + '(' + branch->getStatement() + ')';
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

Value* RuleCallCmpF::value(StateTuple states)
{
    Value* v_ex = branches.at(middle)->value(states);
    StateTuple sigma_ex = branches.at(middle)->sigma(states);
	ComponentFunction* function = env->getComponentFunction(componentName, functionName);
    sigma_ex.declarePState(function->getArgumentName(), v_ex);
	sigma_ex.setAccessCState(true);
    return function->value(sigma_ex);    
}

StateTuple RuleCallCmpF::sigma(StateTuple states)
{
    Value* v_ex = branches.at(middle)->value(states);
    StateTuple sigma_ex = branches.at(middle)->sigma(states);
    vector<StateTuple::declaration> pstate = sigma_ex.getPState();
    ComponentFunction* function = env->getComponentFunction(componentName, functionName);
    sigma_ex.declareCState(function->getArgumentName(), v_ex);
	sigma_ex.setAccessCState(true);
    StateTuple sigma_f = function->sigma(sigma_ex);
	sigma_f.deleteCState(function->getArgumentName());
    vector<StateTuple::declaration> cstate = sigma_f.getCState();
    return *(new StateTuple(pstate, cstate));
}

int RuleCallCmpF::energy(StateTuple states, bool output)
{
    int e_expr = branches.at(middle)->energy(states, output);
    Value* v_ex = branches.at(middle)->value(states);
    StateTuple sigma_ex = branches.at(middle)->sigma(states);
    ComponentFunction* function = env->getComponentFunction(componentName, functionName);
    sigma_ex.declareCState(function->getArgumentName(), v_ex);
	sigma_ex.setAccessCState(true);
    int e_func = function->energy(sigma_ex);
    int time = function->getTime();
    int td_energy = e_expr + td_ec(time, sigma_ex);
    cout << "Time dependent energy usage of " + statement + " is: " << td_energy << endl;
    cout << "Incidental energy usage of " + statement + " is: " << e_func << endl;
    return td_energy + e_func;
}

RuleCallCmpF::~RuleCallCmpF()
{
    //dtor
}
