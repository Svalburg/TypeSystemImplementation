#include "RuleFuncDef.h"

RuleFuncDef::RuleFuncDef(string statement, Rule* left, Rule* right)
{
    this->statement = statement;
    branches.push_back(left);
    branches.push_back(right);
    this->left = 0;
    this->right = 1;
    ruleName = "FuncDef";
}

string RuleFuncDef::toStringSigma(string states)
{
    return branches.at(right)->toStringSigma(states);
}

string RuleFuncDef::toStringE(string states)
{
    return branches.at(right)->toStringSigma(states);
}

string RuleFuncDef::toStringV(string states)
{
    exit(-3);
}

RuleFuncDef::~RuleFuncDef()
{
    //dtor
}
