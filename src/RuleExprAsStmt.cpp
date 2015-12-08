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
    try
    {
        throw runtime_error("Exception: Tried to call toStringV. Not an expression.\n Statement: " + statement);
    }
    catch (const runtime_error& e)
    {
        cout << e.what() << endl;
    }
    exit(-3);
}

int RuleExprAsStmt::value(StateTuple states)
{
    try
    {
        throw runtime_error("Exception: Tried to call value. Not an expression.\n Statement: " + statement);
    }
    catch (const runtime_error& e)
    {
        cout << e.what() << endl;
    }
    exit(-4);
}

StateTuple RuleExprAsStmt::sigma(StateTuple states)
{
    return branches.at(middle)->sigma(states);
}

int RuleExprAsStmt::energy(StateTuple states)
{
    return branches.at(middle)->energy(states);
}

RuleExprAsStmt::~RuleExprAsStmt()
{
    //dtor
}
