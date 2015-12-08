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
    try
    {
        throw runtime_error("Exception: Tried to call toStringV. Not an expression.\n Statement: " + statement);
    }
    catch (const runtime_error& e)
    {
        cout << e.what() << endl;
    }
    exit(-3);
}

int RuleSkip::value(StateTuple states)
{
    try
    {
        throw runtime_error("Exception: Tried to call value. Not an expression.\n Statement: " + statement);
    }
    catch (const runtime_error& e)
    {
        cout << e.what() << endl;
    }
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
