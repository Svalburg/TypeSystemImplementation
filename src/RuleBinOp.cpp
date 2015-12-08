#include "RuleBinOp.h"
#include "Environment.h"

RuleBinOp::RuleBinOp(string binop, Rule* left, Rule* right)
{
    this->statement = left->getStatement() + " " + binop + " " + right->getStatement();
    this->binop = binop;
    ruleName = "BinOp";
    branches.push_back(left);
    branches.push_back(right);
    this->left = 0;
    this->right = 1;
}

string RuleBinOp::toStringSigma()
{
    return branches.at(left)->toStringSigma() + " >>> " + branches.at(right)->toStringSigma();
}

string RuleBinOp::toStringE()
{
    string sigma1 = branches.at(left)->toStringSigma();
    string sigma2 = branches.at(right)->toStringSigma() + "( " + sigma1 + " )";
    string e1 = branches.at(left)->toStringE();
    string e2 = branches.at(right)->toStringE() + "( " + sigma1 + " )";
    return e1 + " + (" + sigma1 + " >>> (" + e2 + " + " + sigma2 + " >>> td_ec(t_" + binop + ") ) )";
}

string RuleBinOp::toStringV()
{
    string v1 = branches.at(left)->toStringV();
    string sigma1 = branches.at(left)->toStringSigma();
    string v2 = sigma1 + "  >>> " + branches.at(right)->toStringV();
    return v1 + ' ' + binop + ' ' + v2;
}

int RuleBinOp::value(StateTuple states)
{
    try
    {
    int v1 = branches.at(left)->value(states);
    StateTuple sigma1 = branches.at(left)->sigma(states);
    int v2 = branches.at(right)->value(sigma1);
    if(binop == "+")
        return v1 + v2;
    else if(binop == "-")
        return v1 - v2;
    else if(binop == "*")
        return v1 * v2;
    else if (binop == ">")
    {
        if(v1 > v2)
            return 1;
        else return 0;
    }
    else if (binop == ">=")
    {
        if(v1 >= v2)
            return 1;
        else return 0;
    }
    else if (binop == "==")
    {
        if(v1 == v2)
            return 1;
        else return 0;
    }
    else if (binop == "!=")
    {
        if(v1 != v2)
            return 1;
        else return 0;
    }
    else if (binop == "<=")
    {
        if(v1 <= v2)
            return 1;
        else return 0;
    }
    else if (binop == "<")
    {
        if(v1 < v2)
            return 1;
        else return 0;
    }
    else if (binop == "and")
    {
        if(v1 != 0 && v2 != 0)
            return 1;
        else return 0;
    }
    else if (binop == "or")
    {
        if(v1 != 0 || v2 != 0)
            return 1;
        else return 0;
    }
    throw runtime_error("Exception: Invalid binary operator" + binop + " in statement: " + statement);
    }
    catch (const runtime_error& e)
    {
        cout << e.what() << endl;
    }
    exit(-5);
}

StateTuple RuleBinOp::sigma(StateTuple states)
{
    StateTuple sigma1 = branches.at(left)->sigma(states);
    StateTuple sigma2 = branches.at(right)->sigma(sigma1);
    return sigma2;
}

int RuleBinOp::energy(StateTuple states)
{
    int e1 = branches.at(left)->energy(states);
    StateTuple sigma1 = branches.at(left)->sigma(states);
    int e2 = branches.at(right)->energy(sigma1);
    int time = env->getTBinop();
    StateTuple sigma2 = branches.at(right)->sigma(sigma1);
    return e1 + e2 + td_ec(time, sigma2);
}

RuleBinOp::~RuleBinOp()
{
    //dtor
}
