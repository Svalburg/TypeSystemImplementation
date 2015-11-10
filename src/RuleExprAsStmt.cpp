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

RuleExprAsStmt::~RuleExprAsStmt()
{
    //dtor
}
