#include "RuleFuncDef.h"
#include "Environment.h"

RuleFuncDef::RuleFuncDef(string functionname, string argumentname, Rule* left, Rule* right)
{
    this->functionname = functionname;
    this->argumentname = argumentname;
    this->statement = "function " + functionname + "(" + argumentname + ") " + left->getStatement() + " end " + right->getStatement();
    branches.push_back(left);
    branches.push_back(right);
    this->left = 0;
    this->right = 1;
    ruleName = "FuncDef";
}

string RuleFuncDef::toStringSigma()
{
    return branches.at(right)->toStringSigma();
}

string RuleFuncDef::toStringE()
{
    return branches.at(right)->toStringSigma();
}

string RuleFuncDef::toStringV()
{
	throw runtime_error("Exception: Tried to call toStringV. Not an expression.\n Statement: " + statement);
}

int RuleFuncDef::value(StateTuple states)
{
	throw runtime_error("Exception: Tried to call value. Not an expression.\n Statement: " + statement);
}

StateTuple RuleFuncDef::sigma(StateTuple states)
{
	return branches.at(right)->sigma(states);
}

int RuleFuncDef::energy(StateTuple states, bool output)
{
	return branches.at(right)->energy(states);
}

void RuleFuncDef::updateEnvironment(Environment* env)
{
    Environment* newenv = env->clone();
    newenv->addFunction(functionname, argumentname, branches.at(left));
	for(size_t i = 0; i < branches.size(); i++)
        branches.at(i)->updateEnvironment(newenv);
}

RuleFuncDef::~RuleFuncDef()
{
    //dtor
}
