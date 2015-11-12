#include <iostream>
#include <stdio.h>
#include <vector>
#include "libtypesystem.h"

using namespace std;

int main()
{
    RuleConst* zero = new RuleConst("0");
    RuleCallCmpF* read = new RuleCallCmpF("meter.Read", zero);
    RuleAssign* assx = new RuleAssign("x", read);
    RuleCallCmpF* on = new RuleCallCmpF("antenna.On", zero);
    RuleVar* x = new RuleVar("x");
    RuleCallCmpF* antqueue = new RuleCallCmpF("antenna.Queue", x);
    RuleCallCmpF* send = new RuleCallCmpF("antenna.Send", zero);
    RuleCallCmpF* receive = new RuleCallCmpF("DataCenter.Receive", zero);
    RuleAssign* assy = new RuleAssign("y", receive);
    RuleVar* y = new RuleVar("y");
    RuleCallCmpF* store = new RuleCallCmpF("DataCenter.Store", y);
    RuleCallCmpF* off = new RuleCallCmpF("antenna.Off", zero);
    RuleStmtConcat* s6 = new RuleStmtConcat(store, off);
    RuleStmtConcat* s5 = new RuleStmtConcat(assy, s6);
    RuleStmtConcat* s4 = new RuleStmtConcat(send, s5);
    RuleStmtConcat* s3 = new RuleStmtConcat(antqueue, s4);
    RuleStmtConcat* s2 = new RuleStmtConcat(on, s3);
    RuleStmtConcat* root = new RuleStmtConcat(assx, s2);
    root->updatePath();
    cout << root->getStatement();
    getchar();
    return 0;
}
