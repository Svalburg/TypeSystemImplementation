#include "RuleIf.h"

RuleIf::RuleIf(Rule* left, Rule* middle, Rule* right)
{
    this-> statement = "if " + left->getStatement() + " then " + middle->getStatement() + " else " + right->getStatement() + " end";
    branches.push_back(left);
    branches.push_back(middle);
    branches.push_back(right);
    this->left = 0;
    this-> middle = 1;
    this->right = 2;
    ruleName = "If";
}

string RuleIf::toStringSigma()
{
    string vex = "V_" + branches.at(left)->getPath();
    string sigmaex = "Sigma_" + branches.at(left)->getPath();
    string sigmat = "Sigma_" + branches.at(middle)->getPath();
    string sigmaf = "Sigma_" + branches.at(right)->getPath();
    return "if(" + vex + ", " + sigmaex + ">>>" + sigmat + ", " + sigmaex + ">>>" + sigmaf + ")";
}

string RuleIf::toStringE()
{
    string e_ex = branches.at(left)->toStringE();
    string sigma = branches.at(left)->toStringSigma();
    string vex = "V_" + branches.at(left)->getPath();
    string sigmaex = "Sigma_" + branches.at(left)->getPath();
    string et = "E_" + branches.at(middle)->getPath();
    string ef = "E_" + branches.at(right)->getPath();
    return e_ex + " + td_ec(t_if)(" + sigma + ") + if(" + vex + ", "
        + sigmaex + ">>>" + et + ", " + sigmaex + ">>>" + ef + ")";
}

string RuleIf::toStringV()
{
    exit(-3);
}

RuleIf::~RuleIf()
{
    //dtor
}
