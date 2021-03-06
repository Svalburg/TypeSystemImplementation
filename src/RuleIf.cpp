#include "RuleIf.h"
#include "Environment.h"

RuleIf::RuleIf(Rule* left, Rule* middle, Rule* right)
{
    this->statement = "if " + left->getStatement() + " then\n" + middle->getStatement() + "\n" +" else\n" + right->getStatement() + "\n" + "end";
    branches.push_back(left);
    branches.push_back(middle);
    branches.push_back(right);
    this->left = 0;
    this-> middle = 1;
    this->right = 2;
    ruleName = "If";
}

string RuleIf::toStringSigma()
{
    string vex = "V_" + branches.at(left)->getPath();
    string sigmaex = "Sigma_" + branches.at(left)->getPath();
    string sigmat = "Sigma_" + branches.at(middle)->getPath();
    string sigmaf = "Sigma_" + branches.at(right)->getPath();
    return "if(" + vex + ", " + sigmaex + ">>>" + sigmat + ", " + sigmaex + ">>>" + sigmaf + ")";
}

string RuleIf::toStringE()
{
    string e_ex = branches.at(left)->toStringE();
    string sigma = branches.at(left)->toStringSigma();
    string vex = "V_" + branches.at(left)->getPath();
    string sigmaex = "Sigma_" + branches.at(left)->getPath();
    string et = "E_" + branches.at(middle)->getPath();
    string ef = "E_" + branches.at(right)->getPath();
    return e_ex + " + td_ec(t_if)(" + sigma + ") + if(" + vex + ", "
        + sigmaex + ">>>" + et + ", " + sigmaex + ">>>" + ef + ")";
}

string RuleIf::toStringV()
{
    throw runtime_error("Exception: Tried to call toStringV. Not an expression.\n Statement: " + statement);
}

Value* RuleIf::value(StateTuple states)
{
    throw runtime_error("Exception: Tried to call value. Not an expression.\n Statement: " + statement);
}

StateTuple RuleIf::sigma(StateTuple states)
{
	StateTuple sigma_ex = branches.at(left)->sigma(states);
    ValueInt* valueInt = dynamic_cast<ValueInt*>(branches.at(left)->value(states));
    if(valueInt)
    {
        if(valueInt->getValue()!= 0)
            return branches.at(middle)->sigma(sigma_ex);
        else
            return branches.at(right)->sigma(sigma_ex);
    }
    else throw runtime_error("Exception: Used non-integer value in if statement: \n" + statement + "\n");
}

int RuleIf::energy(StateTuple states, bool output)
{
	StateTuple sigma_ex = branches.at(left)->sigma(states);
	int e_ex = branches.at(left)->energy(states, output);
	int e_stmt;
    ValueInt* valueInt = dynamic_cast<ValueInt*>(branches.at(left)->value(states));
    if(valueInt)
    {
        if(valueInt->getValue() != 0)
            e_stmt = branches.at(middle)->energy(sigma_ex, output);
        else
            e_stmt = branches.at(right)->energy(sigma_ex, output);
        int tdec = td_ec(env->getTIf(), sigma_ex);
        if(output)
        {
            cout << "Energy usage of \"if " << branches.at(left)->getStatement() << " then\" is: " << tdec << endl; 
        }
        return e_ex + tdec + e_stmt;
    }
    else throw runtime_error("Exception: Used non-integer value in if statement: \n" + statement + "\n");
}

RuleIf::~RuleIf()
{
    //dtor
}
