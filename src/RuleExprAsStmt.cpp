#include "RuleExprAsStmt.h"

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
    exit(-3);
}

int RuleExprAsStmt::value(StateTuple states)
{
    exit(-4);
}

StateTuple RuleExprAsStmt::sigma(StateTuple states)
{
    return branches.at(middle)->sigma(states);
}

RuleExprAsStmt::~RuleExprAsStmt()
{
    //dtor
}
