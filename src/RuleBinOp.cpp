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

string RuleBinOp::toStringSigma(string states)
{
    return branches.at(right)->toStringSigma(branches.at(left)->toStringSigma(states));
}

string RuleBinOp::toStringE(string states)
{
    string sigma1 = branches.at(left)->toStringSigma(states);
    string sigma2 = branches.at(right)->toStringSigma(sigma1);
    string e1 = branches.at(left)->toStringE(states);
    string e2 = branches.at(right)->toStringE(sigma1);
    return e1 + " + " + e2 + " + td_ec(t_" + binop + ")(" + sigma2 + ')';
}

string RuleBinOp::toStringV(string states)
{
    string v1 = branches.at(left)->toStringV(states);
    string sigma1 = branches.at(left)->toStringSigma(states);
    string v2 = branches.at(right)->toStringV(sigma1);
    return v1 + ' ' + binop + ' ' + v2;
}

RuleBinOp::~RuleBinOp()
{
    //dtor
}
