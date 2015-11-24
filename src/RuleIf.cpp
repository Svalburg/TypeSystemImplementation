#include "RuleIf.h"
#include "Environment.h"

RuleIf::RuleIf(Rule* left, Rule* middle, Rule* right)
{
    this->statement = "if " + left->getStatement() + " then\n" + middle->getStatement() + "\n" +" else\n" + right->getStatement() + "\n" + "end";
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

int RuleIf::value(StateTuple states)
{
	exit(-3);
}

StateTuple RuleIf::sigma(StateTuple states)
{
	StateTuple sigma_ex = branches.at(left)->sigma(states);
	if(branches.at(left)->value(states) != 0)
		return branches.at(middle)->sigma(sigma_ex);
	else
		return branches.at(right)->sigma(sigma_ex);
}

int RuleIf::energy(StateTuple states)
{
	StateTuple sigma_ex = branches.at(left)->sigma(states);
	if(branches.at(left)->value(states) != 0)
		return branches.at(middle)->energy(sigma_ex);
	else
		return branches.at(right)->energy(sigma_ex);
}

RuleIf::~RuleIf()
{
    //dtor
}
