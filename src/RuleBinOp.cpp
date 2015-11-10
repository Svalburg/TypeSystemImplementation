#include "RuleBinOp.h"

RuleBinOp::RuleBinOp(string binop, Rule* left, Rule* right)
{
    this->statement = left->getStatement() + " " + binop + " " + right->getStatement();
    this->binop = binop;
    ruleName = "BinOp";
    branches.push_back(left);
    branches.push_back(right);
    this->left = 0;
    this->right = 1;
}

string RuleBinOp::toStringSigma()
{
    return branches.at(left)->toStringSigma() + " >>> " + branches.at(right)->toStringSigma();
}

string RuleBinOp::toStringE()
{
    string sigma1 = branches.at(left)->toStringSigma();
    string sigma2 = branches.at(right)->toStringSigma() + "( " + sigma1 + " )";
    string e1 = branches.at(left)->toStringE();
    string e2 = branches.at(right)->toStringE() + "( " + sigma1 + " )";
    return e1 + " + (" + sigma1 + " >>> (" + e2 + " + " + sigma2 + " >>> td_ec(t_" + binop + ") ) )";
}

string RuleBinOp::toStringV()
{
    string v1 = branches.at(left)->toStringV();
    string sigma1 = branches.at(left)->toStringSigma();
    string v2 = sigma1 + "  >>> " + branches.at(right)->toStringV();
    return v1 + ' ' + binop + ' ' + v2;
}

RuleBinOp::~RuleBinOp()
{
    //dtor
}
