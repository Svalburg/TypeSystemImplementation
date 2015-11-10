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

RuleSkip::~RuleSkip()
{
    //dtor
}
