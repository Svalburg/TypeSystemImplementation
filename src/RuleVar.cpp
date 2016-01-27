#include "RuleVar.h"
#include "Environment.h"

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

Value* RuleVar::value(StateTuple states)
{
	return states.getStateValue(statement);
}

StateTuple RuleVar::sigma(StateTuple states)
{
	return states;
}

int RuleVar::energy(StateTuple states, bool output)
{
	int tdec = td_ec(env->getTVar(), states);
	if(output)
		cout << "Energy usage of \"" << statement << "\" is:" << tdec << endl;
	return tdec;
}

RuleVar::~RuleVar()
{
    //dtor
}
