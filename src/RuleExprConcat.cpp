#include "RuleExprConcat.h"

RuleExprConcat::RuleExprConcat(string statement, Rule* left, Rule* right)
{
    this->statement = statement;
    branches.push_back(left);
    branches.push_back(right);
    this->left  = 0;
    this->right = 1;
    ruleName = "ExprConcat";
}

string RuleExprConcat::toStringSigma(string states)
{
    string sigma1 = branches.at(left)->toStringSigma(states);
    string sigma2 = branches.at(right)->toStringSigma(sigma1);
    return sigma2;
}

string RuleExprConcat::toStringE(string states)
{
    string sigma1 = branches.at(left)->toStringSigma(states);
    string e1 = branches.at(left)->toStringE(states);
    string e2 = branches.at(right)->toStringE(sigma1);
    return e1 + " + " + e2;
}

string RuleExprConcat::toStringV(string states)
{
    string sigma1 = branches.at(left)->toStringSigma(states);
    string v = branches.at(right)->toStringV(sigma1);
    return v;
}

RuleExprConcat::~RuleExprConcat()
{
    //dtor
}
