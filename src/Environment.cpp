#include "Environment.h"

Environment::Environment(int t_input, int t_const, int t_var, int t_assign, int t_binop, int t_if,
                int t_while,
                vector<ComponentFunction*> componentFunctions,
                vector<Function*> functions,
                vector<TimeDependentEC*> tdecList)
{
    this->t_input = t_input;
    this->t_const = t_const;
    this->t_var = t_var;
    this->t_assign = t_assign;
    this->t_binop = t_binop;
	this->t_if = t_if;
    this->t_while = t_while;
    this->componentFunctions = componentFunctions;
    this->functions = functions;
    this->tdecList = tdecList;
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

int Environment::getTIf()
{
	return t_if;
}

int Environment::getTWhile()
{
    return t_while;
}

ComponentFunction* Environment::getComponentFunction(string component, string name)
{
	for(size_t i=0;i<componentFunctions.size();i++)
	{
		ComponentFunction* cfunction = componentFunctions.at(i);
		if(cfunction->getComponent() == component && cfunction->getName() == name)
			return cfunction;
	}
	throw runtime_error("Exception: No component function found with name: " + component + '.' + name);
}

Function* Environment::getFunction(string name)
{
	for(size_t i=0;i<functions.size();i++)
	{
		Function* function = functions.at(i);
		if(function->getName() == name)
		{
			return function;
		}
	}
	throw runtime_error("Exception: No function found with name: " + name);
}

TimeDependentEC* Environment::getTimeDependentEC(string componentState)
{
	for(size_t i=0;i<tdecList.size();i++)
	{
		TimeDependentEC* timedependent = tdecList.at(i);
		if(timedependent->getComponentState() == componentState)
			return timedependent;
	}
	throw runtime_error("Exception: No component state found with name: " + componentState);
}

Environment* Environment::clone()
{
    vector<Function*> functioncopy;
    vector<ComponentFunction*> compfunccopy;
    vector<TimeDependentEC*> tdeccopy;
    for(size_t i = 0; i < functions.size(); i++)
        functioncopy.push_back(functions.at(i));
    for(size_t i = 0; i < componentFunctions.size(); i++)
        compfunccopy.push_back(componentFunctions.at(i));
    for(size_t i = 0; i < tdecList.size(); i++)
        tdeccopy.push_back(tdecList.at(i));
    return new Environment(t_input, t_const, t_var, t_assign, t_binop, t_if, t_while, compfunccopy, functioncopy, tdeccopy);
}

void Environment::addFunction(string name, string argumentName, Rule* definition)
{
    Function* newfunction = new Function(name, argumentName, definition);
    functions.push_back(newfunction);
}

Environment::~Environment()
{
}

