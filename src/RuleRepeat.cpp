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
    throw runtime_error("Exception: Tried to call toStringV. Not an expression.\n Statement: " + statement);
}

int RuleRepeat::value(StateTuple states)
{
    throw runtime_error("Exception: Tried to call value. Not an expression.\n Statement: " + statement);
}

StateTuple RuleRepeat::sigma(StateTuple states)
{
	StateTuple states_now = branches.at(left)->sigma(states);
	for(int i = branches.at(left)->value(states); i > 0; i--)
		states_now = branches.at(right)->sigma(states_now);
	return states_now;
}

int RuleRepeat::energy(StateTuple states, bool output)
{
	int E_ex = branches.at(left)->energy(states);
	StateTuple states_now = branches.at(left)->sigma(states);
	int repeatcost = 0;
	for(int i = branches.at(left)->value(states); i > 0; i--)
	{
		repeatcost += branches.at(right)->energy(states_now, false);
		states_now = branches.at(right)->sigma(states_now);
	}
	cout << "Total energy usage of loop \"repeat " << branches.at(left)->getStatement() << " begin\" is: " << repeatcost << endl;
	return E_ex + repeatcost;
}

RuleRepeat::~RuleRepeat()
{
    //dtor
}
