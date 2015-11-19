#include "Environment.h"

Environment::Environment(int t_input, int t_const, int t_var, int t_assign, int t_binop,
                vector<ComponentFunction*> componentFunctions,
                vector<Function*> functions,
                vector<TimeDependentEC*> timeDependent)
{
    this->t_input = t_input;
    this->t_const = t_const;
    this->t_var = t_var;
    this->t_assign = t_assign;
    this->t_binop = t_binop;
    this->componentFunctions = componentFunctions;
    this->functions = functions;
    this->timeDependentEC = timeDependentEC;
}

int Environment::getTInput()
{
    return t_input;
}

int Environment::getTConst()
{
    return t_const;
}

int Environment::getTVar()
{
    return t_var;
}

int Environment::getTAssign()
{
    return t_assign;
}

int Environment::getTBinop()
{
    return t_binop;
}

ComponentFunction* Environment::getComponentFunction(string component, string name)
{
    for(size_t i=0;i<componentFunctions.size();i++)
    {
        ComponentFunction* cfunction = componentFunctions.at(i);
        if(cfunction->getComponent() == component && cfunction->getName() == name)
            return cfunction;
    }
    exit(-6);
}

Function* Environment::getFunction(string name)
{    
    for(size_t i=0;i<functions.size();i++)
    {
        Function* function = functions.at(i);
        if(function->getName() == name)
            return function;
    }
    exit(-6);
}

TimeDependentEC* Environment::getTimeDependentEC(string componentState)
{
    for(size_t i=0;i<timeDependentEC.size();i++)
    {
        TimeDependentEC* timedependent = timeDependentEC.at(i);
        if(timedependent->getComponentState() == componentState)
            return timedependent;
    }
    exit(-6);
}

Environment Environment::clone()
{
    return *(new Environment(t_input, t_const, t_var, t_assign, t_binop, 
        componentFunctions, functions, timeDependentEC));
}

void Environment::addFunction(string name, string argumentName, Rule* definition)
{
    Function* newfunction = new Function(name, argumentName, definition);
    functions.push_back(newfunction);
}

Environment::~Environment()
{
}

