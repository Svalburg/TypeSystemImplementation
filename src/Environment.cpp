#include "Environment.h"

Environment::Environment(int t_input, int t_const, int t_var, int t_assign, int t_binop,
                vector<ComponentFunction*> componentFunctions,
                vector<Function*> functions,
                vector<TimeDependentEC*> tdecList)
{
    this->t_input = t_input;
    this->t_const = t_const;
    this->t_var = t_var;
    this->t_assign = t_assign;
    this->t_binop = t_binop;
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

ComponentFunction* Environment::getComponentFunction(string component, string name)
{
    for(size_t i=0;i<componentFunctions.size();i++)
    {
        ComponentFunction* cfunction = componentFunctions.at(i);
        if(cfunction->getComponent() == component && cfunction->getName() == name)
            return cfunction;
    }
	cout << "No component function found with name: " + component + '.' + name;
	getchar();
    exit(-6);
}

Function* Environment::getFunction(string name)
{    
    for(size_t i=0;i<functions.size();i++)
    {
		cout << "test" << endl;
        Function* function = functions.at(i);
		cout << "test" << endl;
        if(function->getName() == name)
		{
			cout << "test" << endl;
            return function;
		}
    }
	cout << "No function found with name: " + name;
	getchar();
    exit(-6);
}

TimeDependentEC* Environment::getTimeDependentEC(string componentState)
{
    for(size_t i=0;i<tdecList.size();i++)
    {
        TimeDependentEC* timedependent = tdecList.at(i);
        if(timedependent->getComponentState() == componentState)
            return timedependent;
    }
	cout << "No component state found with name: " + componentState;
    exit(-6);
}

Environment Environment::clone()
{
    vector<Function*> functioncopy;
    vector<ComponentFunction*> compfunccopy;
    vector<TimeDependentEC*> tdeccopy;
    for(size_t i = 0; i < functions.size(); i++)
    {
        functioncopy.push_back(functions.at(i));
        compfunccopy.push_back(componentFunctions.at(i));
        tdeccopy.push_back(tdecList.at(i));
    }
    return *(new Environment(t_input, t_const, t_var, t_assign, t_binop, 
        compfunccopy, functioncopy, tdeccopy));
}

void Environment::addFunction(string name, string argumentName, Rule* definition)
{
    Function* newfunction = new Function(name, argumentName, definition);
    functions.push_back(newfunction);
}

Environment::~Environment()
{
}

