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
    throw runtime_error("Tried to call toStringV. Not an expression.\n Statement: " + statement);
    exit(-3);
}

int RuleSkip::value(StateTuple states)
{
    throw runtime_error("Tried to call value. Not an expression.\n Statement: " + statement);
	exit(-4);
}

StateTuple RuleSkip::sigma(StateTuple states)
{
	return states;
}

int RuleSkip::energy(StateTuple states)
{
	return 0;
}

RuleSkip::~RuleSkip()
{
    //dtor
}
