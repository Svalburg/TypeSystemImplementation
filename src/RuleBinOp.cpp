#include "RuleBinOp.h"

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
    int v1 = branches.at(left)->value(states);
    StateTuple sigma1 = branches.at(left)->sigma(states);
    int v2 = branches.at(right)->values(sigma1);
    switch(binop)
    {
        case "+": return v1 + v2; break;
        case "-": return v1 - v2; break;
        case "*": return v1 * v2; break;
        case ">": if(v1 > v2){return 1;} 
            else {return 0;} break; 
        case ">=": if(v1 >= v2){return 1;} 
            else {return 0;} break; 
        case "==": if(v1 == v2){return 1;} 
            else {return 0;} break;
        case "!=": if(v1!= v2){return 1;} 
            else {return 0;} break; 
        case "<=": if(v1 <= v2){return 1;} 
            else {return 0;} break; 
        case "<": if(v1 < v2){return 1;} 
            else {return 0;} break; 
        case "and": if(v1 != 0 && v2 != 0){return 1;} 
            else {return 0;} break; 
        case "or": if(v1 != 0 || v2 != 0){return 1;} 
            else {return 0;} break; 
    }
    exit(-5);
}

StateTuple RuleBinOp::sigma(StateTuple states)
{
    StateTuple sigma1 = branches.at(left)->sigma(states);
    StateTuple sigma2 = branches.at(right)->sigma(sigma1);
    return sigma2;
}

RuleBinOp::~RuleBinOp()
{
    //dtor
}
