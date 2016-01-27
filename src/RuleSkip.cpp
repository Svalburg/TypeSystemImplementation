#include "RuleSkip.h"
#include "Environment.h"

RuleSkip::RuleSkip()
{
    statement = "skip";
    ruleName = "Skip";
}

string RuleSkip::toStringSigma()
{
    return "id";
}

string RuleSkip::toStringE()
{
    return "zero";
}

string RuleSkip::toStringV()
{
    throw runtime_error("Exception: Tried to call toStringV. Not an expression.\n Statement: " + statement);
}

Value* RuleSkip::value(StateTuple states)
{
    throw runtime_error("Exception: Tried to call value. Not an expression.\n Statement: " + statement);
}

StateTuple RuleSkip::sigma(StateTuple states)
{
	return states;
}

int RuleSkip::energy(StateTuple states, bool output)
{
	return 0;
}

RuleSkip::~RuleSkip()
{
    //dtor
}
