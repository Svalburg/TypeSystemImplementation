#include "RuleInput.h"
#include "Environment.h"

RuleInput::RuleInput(string statement)
{
    this->statement = statement;
    ruleName = "Input";
}

string RuleInput::toStringSigma()
{
    return "id";
}

string RuleInput::toStringE()
{
    return "td_ec(t_input)";
}

string RuleInput::toStringV()
{
    return "Lookup_" + statement;
}

int RuleInput::value(StateTuple states)
{
	return states.getPStateValue(statement);
}

StateTuple RuleInput::sigma(StateTuple states)
{
	return states;
}

int RuleInput::energy(StateTuple states, bool output)
{
	int tdec = td_ec(env->getTInput(), states);
	if(output)
		cout << "Energy usage of \"" << statement << "\" is:" << tdec << endl;
	return tdec;
}

RuleInput::~RuleInput()
{
    //dtor
}
