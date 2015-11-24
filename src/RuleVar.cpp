#include "RuleVar.h"

RuleVar::RuleVar(string statement)
{
    this->statement = statement;
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
