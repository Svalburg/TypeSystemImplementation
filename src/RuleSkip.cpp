#include "RuleSkip.h"

RuleSkip::RuleSkip()
{
    statement = "skip";
    ruleName = "Skip";
}

string RuleSkip::toStringSigma()
{
    return states;
}

string RuleSkip::toStringE()
{
    return "nil";
}

string RuleSkip::toStringV()
{
    exit(-3);
}

RuleSkip::~RuleSkip()
{
    //dtor
}
