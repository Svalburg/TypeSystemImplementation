#include "RuleFuncDef.h"

RuleFuncDef::RuleFuncDef(string functionname, Rule* left, Rule* right)
{
    this->statement = "function " + functionname + " begin " + left->getStatement() + " end " + right->getStatement();
    branches.push_back(left);
    branches.push_back(right);
    this->left = 0;
    this->right = 1;
    ruleName = "FuncDef";
}

string RuleFuncDef::toStringSigma()
{
    return branches.at(right)->toStringSigma();
}

string RuleFuncDef::toStringE()
{
    return branches.at(right)->toStringSigma();
}

string RuleFuncDef::toStringV()
{
    exit(-3);
}

RuleFuncDef::~RuleFuncDef()
{
    //dtor
}
