#include "RuleExprAsStmt.h"
#include "Environment.h"

RuleExprAsStmt::RuleExprAsStmt(Rule* branch)
{
    statement = branch->getStatement();
    branches.push_back(branch);
    middle = 0;
    ruleName = "ExprAsStmt";
}

string RuleExprAsStmt::toStringSigma()
{
    return branches.at(middle)->toStringSigma();
}

string RuleExprAsStmt::toStringE()
{
    return branches.at(middle)->toStringE();
}

string RuleExprAsStmt::toStringV()
{
	throw runtime_error("Exception: Tried to call toStringV. Not an expression.\n Statement: " + statement);
}

int RuleExprAsStmt::value(StateTuple states)
{
	throw runtime_error("Exception: Tried to call value. Not an expression.\n Statement: " + statement);
}

StateTuple RuleExprAsStmt::sigma(StateTuple states)
{
    return branches.at(middle)->sigma(states);
}

int RuleExprAsStmt::energy(StateTuple states, bool output)
{
    return branches.at(middle)->energy(states);
}

RuleExprAsStmt::~RuleExprAsStmt()
{
    //dtor
}
