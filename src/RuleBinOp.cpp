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

Value* RuleBinOp::value(StateTuple states)
{
    StateTuple sigma1 = branches.at(left)->sigma(states);
    ValueInt* valueInt1 = dynamic_cast<ValueInt*>(branches.at(left)->value(states));
    ValueInt* valueInt2 = dynamic_cast<ValueInt*>(branches.at(right)->value(sigma1));
    ValueString* valueString1 = dynamic_cast<ValueString*>(branches.at(left)->value(states));
    ValueString* valueString2 = dynamic_cast<ValueString*>(branches.at(right)->value(sigma1));
    if(valueInt1 && valueInt2)
    {
        int v1 = valueInt1->getValue();
        int v2 = valueInt2->getValue();
        if(binop == "+")
            return new ValueInt(v1 + v2);
        else if(binop == "-")
            return new ValueInt(v1 - v2);
        else if(binop == "*")
            return new ValueInt(v1 * v2);
        else if (binop == ">")
        {
            if(v1 > v2)
                return new ValueInt(1);
            else return new ValueInt(0);
        }
        else if (binop == ">=")
        {
            if(v1 >= v2)
                return new ValueInt(1);
            else return new ValueInt(0);
        }
        else if (binop == "==")
        {
            if(v1 == v2)
                return new ValueInt(1);
            else return new ValueInt(0);
        }
        else if (binop == "!=")
        {
            if(v1 != v2)
                return new ValueInt(1);
            else return new ValueInt(0);
        }
        else if (binop == "<=")
        {
            if(v1 <= v2)
                return new ValueInt(1);
            else return new ValueInt(0);
        }
        else if (binop == "<")
        {
            if(v1 < v2)
                return new ValueInt(1);
            else return new ValueInt(0);
        }
        else if (binop == "and" || binop == "&&")
        {
            if(v1 != 0 && v2 != 0)
                return new ValueInt(1);
            else return new ValueInt(0);
        }
        else if (binop == "or" || binop == "||")
        {
            if(v1 != 0 || v2 != 0)
                return new ValueInt(1);
            else return new ValueInt(0);
        }
        throw runtime_error("Exception: Invalid binary operator" + binop + " in statement: " + statement);
    }
    else if(valueString1 && valueString2)
    {
        string v1 = valueString1->getValue();
        string v2 = valueString2->getValue();
        if(binop == "+")
            return new ValueString(v1 + v2);
        else if (binop == "==")
        {
            if(v1.compare(v2) == 0)
                return new ValueInt(1);
            else return new ValueInt(0);
        }
        else if (binop == "!=")
        {
            if(v1.compare(v2) != 0)
                return new ValueInt(1);
            else return new ValueInt(0);
        }
        throw runtime_error("Exception: Invalid binary operator" + binop + " in statement: " + statement);
    }
    else throw runtime_error("Exception: Incompatible types in statement: \n" + statement + "\n");
}

StateTuple RuleBinOp::sigma(StateTuple states)
{
    StateTuple sigma1 = branches.at(left)->sigma(states);
    StateTuple sigma2 = branches.at(right)->sigma(sigma1);
    return sigma2;
}

int RuleBinOp::energy(StateTuple states, bool output)
{
    int e1 = branches.at(left)->energy(states, false);
    StateTuple sigma1 = branches.at(left)->sigma(states);
    int e2 = branches.at(right)->energy(sigma1, false);
    int time = env->getTBinop();
    StateTuple sigma2 = branches.at(right)->sigma(sigma1);
    int total_energy = e1 + e2 + td_ec(time, sigma2);
    if(output)
        cout << "Energy usage of " + statement + " is: " << total_energy << endl; 
    return total_energy;
}

RuleBinOp::~RuleBinOp()
{
    //dtor
}
