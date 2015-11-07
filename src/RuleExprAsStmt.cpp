#include "RuleExprAsStmt.h"

RuleExprAsStmt::RuleExprAsStmt(Rule* branch)
{
    statement = branch->getStatement();
    branches.push_back(branch);
    middle = 0;
    ruleName = "ExprAsStmt";
}

string RuleExprAsStmt::toStringSigma(string states)
{
    return branches.at(middle)->toStringSigma(states);
}

string RuleExprAsStmt::toStringE(string states)
{
    return branches.at(middle)->toStringE(states);
}

string RuleExprAsStmt::toStringV(string states)
{
    exit(-3);
}

RuleExprAsStmt::~RuleExprAsStmt()
{
    //dtor
}
