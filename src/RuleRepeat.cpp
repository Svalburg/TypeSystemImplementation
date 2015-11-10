#include "RuleRepeat.h"

RuleRepeat::RuleRepeat(Rule* left, Rule* right)
{
    this->statement = "repeat " + left->getStatement() + " begin " + right->getStatement() + " end";
    branches.push_back(left);
    branches.push_back(right);
    this->left = 0;
    this->right = 1;
    ruleName = "Repeat";
}

string RuleRepeat::toStringSigma(string states)
{
    string vex = "V_" + branches.at(left)->getPath();
    string sigmaex = "Sigma_" + branches.at(left)->getPath();
    string sigmast = "Sigma_" + branches.at(right)->getPath();
    return "repeat_v(" + vex + ", " + sigmaex + ", " + sigmast + ")(" + states + ')';
}

string RuleRepeat::toStringE(string states)
{
    string vex = "V_" + branches.at(left)->getPath();
    string sigmaex = "Sigma_" + branches.at(left)->getPath();
    string e_ex = branches.at(left)->toStringE(states);
    string est = "E_" + branches.at(right)->getPath();
    string sigmast = "Sigma_" + branches.at(right)->getPath();
    return e_ex + " + repeat_ex(" + vex + ", " + sigmaex + ", " + est + ", " + sigmast + ")(" + states + ')';
}

string RuleRepeat::toStringV(string states)
{
    exit(-3);
}

RuleRepeat::~RuleRepeat()
{
    //dtor
}
