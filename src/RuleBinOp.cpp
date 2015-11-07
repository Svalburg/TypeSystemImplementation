#include "RuleBinOp.h"

RuleBinOp::RuleBinOp(string statement, Rule* left, Rule* right)
{
    this->statement = statement;
    ruleName = "BinOp";
    branches.push_back(left);
    branches.push_back(right);
    this->left = 0;
    this->right = 1;

    //remove the left branch statement and the space
    string remainder = statement.substr(branches.at(this->left)->getStatement().size()+1);
    //find and remove the second space and statement, the remainder is the binop we need
    size_t found = remainder.find(branches.at(this->right)->getStatement());
    if(found != string::npos)
        binop = remainder.erase(found-1);
    else
        exit(-1);
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
