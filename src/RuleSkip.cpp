#include "RuleSkip.h"

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
    exit(-3);
}

int RuleSkip::value(StateTuple states)
{
	exit(-3);
}

StateTuple RuleSkip::sigma(StateTuple states)
{
	return states;
}

RuleSkip::~RuleSkip()
{
    //dtor
}
