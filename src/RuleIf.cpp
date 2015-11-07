#include "RuleIf.h"

RuleIf::RuleIf(string statement, Rule* left, Rule* middle, Rule* right)
{
    this-> statement = statement;
    branches.push_back(left);
    branches.push_back(middle);
    branches.push_back(right);
    this->left = 0;
    this-> middle = 1;
    this->right = 2;
    ruleName = "If";
}

string RuleIf::toStringSigma(string states)
{
    string vex = "V_" + branches.at(left)->getPath();
    string sigmaex = "Sigma_" + branches.at(left)->getPath();
    string sigmat = "Sigma_" + branches.at(middle)->getPath();
    string sigmaf = "Sigma_" + branches.at(right)->getPath();
    return "if(" + vex + ", " + sigmaex + ">>>" + sigmat + ", " + sigmaex + ">>>" + sigmaf + ")(" + states + ')';
}

string RuleIf::toStringE(string states)
{
    string e_ex = branches.at(left)->toStringE(states);
    string sigma = branches.at(left)->toStringSigma(states);
    string vex = "V_" + branches.at(left)->getPath();
    string sigmaex = "Sigma_" + branches.at(left)->getPath();
    string et = "E_" + branches.at(middle)->getPath();
    string ef = "E_" + branches.at(right)->getPath();
    return e_ex + " + td_ec(t_if)(" + sigma + ") + if(" + vex + ", "
        + sigmaex + ">>>" + et + ", " + sigmaex + ">>>" + ef + ")(" + states + ')';
}

string RuleIf::toStringV(string states)
{
    exit(-3);
}

RuleIf::~RuleIf()
{
    //dtor
}
