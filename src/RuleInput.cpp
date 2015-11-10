#include "RuleInput.h"

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

RuleInput::~RuleInput()
{
    //dtor
}
