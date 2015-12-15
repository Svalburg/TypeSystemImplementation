#include "RuleWhile.h"
#include "Environment.h"

RuleWhile::RuleWhile(Rule* left, Rule* right)
{
    this->statement = "While " + left->getStatement() + " begin\n" + right->getStatement() + "\n" + "end";
    branches.push_back(left);
    branches.push_back(right);
    this->left = 0;
    this->right = 1;
    ruleName = "While";
}

string RuleWhile::toStringSigma()
{
    string vex = "V_" + branches.at(left)->getPath();
    string sigmaex = "Sigma_" + branches.at(left)->getPath();
    string sigmast = "Sigma_" + branches.at(right)->getPath();
    return "while_v(" + vex + ", " + sigmaex + ", " + sigmast + ")";
}

string RuleWhile::toStringE()
{
    string vex = "V_" + branches.at(left)->getPath();
    string sigmaex = "Sigma_" + branches.at(left)->getPath();
    string e_ex = branches.at(left)->toStringE();
    string est = "E_" + branches.at(right)->getPath();
    string sigmast = "Sigma_" + branches.at(right)->getPath();
    return "while_ex(" + vex + ", " + sigmaex + ", " + est + ", " + sigmast + ")";
}

string RuleWhile::toStringV()
{
    throw runtime_error("Exception: Tried to call toStringV. Not an expression.\n Statement: " + statement);
}

int RuleWhile::value(StateTuple states)
{
    throw runtime_error("Exception: Tried to call value. Not an expression.\n Statement: " + statement);
}

StateTuple RuleWhile::sigma(StateTuple states)
{
	int v = branches.at(left)->value(states);
	StateTuple states_now = branches.at(left)->sigma(states);
	while(v != 0)
	{
		states_now = branches.at(right)->sigma(states_now);
		v = branches.at(left)->value(states_now);
		states_now = branches.at(left)->sigma(states_now);
	}
	return states_now;
}

int RuleWhile::energy(StateTuple states, bool output)
{
	int e_ex = branches.at(left)->energy(states, false);
	int v = branches.at(left)->value(states);
	StateTuple states_now = branches.at(left)->sigma(states);
	int whilecost = e_ex + td_ec(env->getTIf(), states);
	while(v != 0)
	{
		whilecost += branches.at(right)->energy(states_now, false);
		states_now = branches.at(right)->sigma(states_now);
		v = branches.at(left)->value(states_now);
		whilecost += branches.at(left)->energy(states_now, false) + td_ec(env->getTIf(), states_now);
		states_now = branches.at(left)->sigma(states_now);
	}
	cout << "Total energy usage of loop \"while " << branches.at(left)->getStatement() << " begin\" is: " << whilecost << endl;
	return whilecost;
}

RuleWhile::~RuleWhile()
{
    //dtor
}
