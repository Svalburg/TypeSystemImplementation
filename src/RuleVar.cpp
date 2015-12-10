#include "RuleVar.h"
#include "Environment.h"

RuleVar::RuleVar(string statement, bool readComponentState)
{
    this->statement = statement;
	this->readComponentState = readComponentState;
    ruleName = "Var";
}

string RuleVar::toStringSigma()
{
    return "id";
}

string RuleVar::toStringE()
{
    return "td_ec(t_var)";
}

string RuleVar::toStringV()
{
    return "Lookup_" + statement;
}

int RuleVar::value(StateTuple states)
{
	if(readComponentState)
		return states.getCStateValue(statement)
	else
		return states.getPStateValue(statement);
}

StateTuple RuleVar::sigma(StateTuple states)
{
	return states;
}

int RuleVar::energy(StateTuple states)
{
	return td_ec(env->getTVar(), states);
}

RuleVar::~RuleVar()
{
    //dtor
}
