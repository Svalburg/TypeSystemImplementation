#include "RuleSkip.h"

RuleSkip::RuleSkip()
{
    statement = "skip";
    ruleName = "Skip";
}

string RuleSkip::toStringSigma(string states)
{
    return states;
}

string RuleSkip::toStringE(string states)
{
    return "nil(" + states + ')';
}

string RuleSkip::toStringV(string states)
{
    exit(-3);
}

RuleSkip::~RuleSkip()
{
    //dtor
}
