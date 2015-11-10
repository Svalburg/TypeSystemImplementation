#include "RuleInput.h"

RuleInput::RuleInput(string statement)
{
    this->statement = statement;
    ruleName = "Input";
}

string RuleInput::toStringSigma()
{
    return states;
}

string RuleInput::toStringE()
{
    return "td_ec(t_input)";
}

string RuleInput::toStringV()
{
    return "Lookup_" + statement + '(' + states + ')';
}

RuleInput::~RuleInput()
{
    //dtor
}
