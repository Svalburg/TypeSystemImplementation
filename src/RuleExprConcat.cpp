#include "RuleExprConcat.h"

RuleExprConcat::RuleExprConcat(Rule* left, Rule* right)
{
    this->statement = left->getStatement() + "," + right->getStatement();
    branches.push_back(left);
    branches.push_back(right);
    this->left  = 0;
    this->right = 1;
    ruleName = "ExprConcat";
}

string RuleExprConcat::toStringSigma()
{
    string sigma1 = branches.at(left)->toStringSigma();
    string sigma2 = branches.at(right)->toStringSigma();
    return sigma1 + " >>> " + sigma2;
}

string RuleExprConcat::toStringE()
{
    string sigma1 = branches.at(left)->toStringSigma();
    string e1 = branches.at(left)->toStringE();
    string e2 = branches.at(right)->toStringE();
    return e1 + " + (" + sigma1 + " >>> " + e2 + ')';
}

string RuleExprConcat::toStringV()
{
    string sigma1 = branches.at(left)->toStringSigma();
    string v = branches.at(right)->toStringV();
    return sigma1 + " >>> " + v;
}

RuleExprConcat::~RuleExprConcat()
{
    //dtor
}
