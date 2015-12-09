#include "RuleStmtConcat.h"
#include "Environment.h"

RuleStmtConcat::RuleStmtConcat(Rule* left, Rule* right)
{
    this->statement = left->getStatement() + ";\n" + right->getStatement();
    branches.push_back(left);
    branches.push_back(right);
    this->left  = 0;
    this->right = 1;
    ruleName = "StmtConcat";
}

string RuleStmtConcat::toStringSigma()
{
    string sigma1 = branches.at(left)->toStringSigma();
    string sigma2 = branches.at(right)->toStringSigma();
    return sigma1 + " >>> " + sigma2;
}

string RuleStmtConcat::toStringE()
{
    string sigma1 = branches.at(left)->toStringSigma();
    string e1 = branches.at(left)->toStringE();
    string e2 = branches.at(right)->toStringE();
    return e1 + " + \n( " + sigma1 + " >>> " + e2 + " )";
}

string RuleStmtConcat::toStringV()
{
    throw runtime_error("Exception: Tried to call toStringV. Not an expression.\n Statement: " + statement);
}

int RuleStmtConcat::value(StateTuple states)
{
    throw runtime_error("Exception: Tried to call value. Not an expression.\n Statement: " + statement);
}

StateTuple RuleStmtConcat::sigma(StateTuple states)
{
	StateTuple sigma1 = branches.at(left)->sigma(states);
	return branches.at(right)->sigma(sigma1);
}

int RuleStmtConcat::energy(StateTuple states)
{
	StateTuple sigma1 = branches.at(left)->sigma(states);
	return branches.at(left)->energy(states) + branches.at(right)->energy(sigma1);
}

RuleStmtConcat::~RuleStmtConcat()
{
    //dtor
}
