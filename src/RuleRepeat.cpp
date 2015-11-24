#include "RuleRepeat.h"
#include "Environment.h"

RuleRepeat::RuleRepeat(Rule* left, Rule* right)
{
    this->statement = "repeat " + left->getStatement() + " begin\n" + right->getStatement() + "\n" + "end";
    branches.push_back(left);
    branches.push_back(right);
    this->left = 0;
    this->right = 1;
    ruleName = "Repeat";
}

string RuleRepeat::toStringSigma()
{
    string vex = "V_" + branches.at(left)->getPath();
    string sigmaex = "Sigma_" + branches.at(left)->getPath();
    string sigmast = "Sigma_" + branches.at(right)->getPath();
    return "repeat_v(" + vex + ", " + sigmaex + ", " + sigmast + ")";
}

string RuleRepeat::toStringE()
{
    string vex = "V_" + branches.at(left)->getPath();
    string sigmaex = "Sigma_" + branches.at(left)->getPath();
    string e_ex = branches.at(left)->toStringE();
    string est = "E_" + branches.at(right)->getPath();
    string sigmast = "Sigma_" + branches.at(right)->getPath();
    return e_ex + " + repeat_ex(" + vex + ", " + sigmaex + ", " + est + ", " + sigmast + ")";
}

string RuleRepeat::toStringV()
{
    exit(-3);
}

int RuleRepeat::value(StateTuple states)
{
	exit(-3);
}

StateTuple RuleRepeat::sigma(StateTuple states)
{
	StateTuple states_now = branches.at(left)->sigma(states);
	for(int i = branches.at(left)->value(states); i > 0; i--)
		states_now = branches.at(right)->sigma(states_now);
	return states_now;
}

int RuleRepeat::energy(StateTuple states)
{
	int E_ex = branches.at(left)->energy(states);
	StateTuple states_now = branches.at(left)->sigma(states);
	int repeatcost = 0;
	for(int i = branches.at(left)->value(states); i > 0; i--)
	{
		repeatcost += branches.at(right)->energy(states_now);
		states_now = branches.at(right)->sigma(states_now);
	}
	return E_ex + repeatcost;
}

RuleRepeat::~RuleRepeat()
{
    //dtor
}
