#include <iostream>
#include <stdio.h>
#include <vector>
#include "Rule.h"
#include "RuleInput.h"
#include "RuleVar.h"
#include "RuleBinOp.h"
#include "RuleConst.h"
#include "RuleAssign.h"
#include "RuleIf.h"

using namespace std;

int main()
{
    RuleConst* branch1 = new RuleConst("5");
    RuleConst* branch2 = new RuleConst("3");
    RuleVar* branch3 = new RuleVar("y");
    RuleAssign* t = new RuleAssign("x", branch1);
    RuleAssign* e = new RuleAssign("x", branch2);
    RuleIf* root = new RuleIf(branch3, t, e);
    root->updatePath();
    cout << root->getStatement();
    getchar();
    return 0;
}
