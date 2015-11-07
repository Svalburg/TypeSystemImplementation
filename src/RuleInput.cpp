#include "RuleInput.h"

RuleInput::RuleInput(string statement)
{
    this->statement = statement;
    ruleName = "Input";
}

string RuleInput::toStringSigma(string states)
{
    return states;
}

string RuleInput::toStringE(string states)
{
    return "td_ec(t_input)(" + states + ')';
}

string RuleInput::toStringV(string states)
{
    return "Lookup_" + statement + '(' + states + ')';
}

RuleInput::~RuleInput()
{
    //dtor
}
