#include <iostream>
#include <stdio.h>
#include <vector>
#include "libtypesystem.h"

using namespace std;

int main()
{
	/*RuleConst* three = new RuleConst("3");
	RuleConst* zero = new RuleConst("0");
	RuleCallCmpF* test = new RuleCallCmpF("test", "testing", zero);
	RuleRepeat* repeat = new RuleRepeat(three, test);
	RuleExprAsStmt* repeatstmt = new RuleExprAsStmt(repeat);
	RuleSkip* skip = new RuleSkip();
	RuleStmtConcat* root = new RuleStmtConcat(repeatstmt, skip);
	
	RuleSkip* skip2 = new RuleSkip();
	RuleConst* forty = new RuleConst("40");
	ComponentFunction* func = new ComponentFunction("test", "testing", "x", skip2, forty, 5);
	
	RuleConst* five = new RuleConst("5");
	TimeDependentEC* testdraw = new TimeDependentEC("testState", five);
	
	vector<ComponentFunction*> compfuncs;
	vector<Function*> funcs;
	vector<TimeDependentEC*> tdec;
	compfuncs.push_back(func);
	tdec.push_back(testdraw);
	Environment* env = new Environment(1, 2, 3, 4, 5, compfuncs, funcs, tdec);
	
	root->updateEnvironment(env);
	root->updatePath();
	
	StateTuple* states = new StateTuple();
	states->declareCState("testState", 0);
	
    cout << root->getStatement() << endl;
	cout << "Energy usage is : " << root->energy(*states);*/
    
    /*RuleConst* const1 = new RuleConst("3");
    RuleAssign* assign1 = new RuleAssign("x", const1);
    RuleExprAsStmt* expr1 = new RuleExprAsStmt(assign1);
    RuleVar* varx = new RuleVar("x");
    RuleConst* const2 = new RuleConst("6");
    RuleBinOp* binop1 = new RuleBinOp("<", varx, const2);
    RuleConst* const3 = new RuleConst("3");
    RuleAssign* assign2 = new RuleAssign("x", const3);
    RuleExprAsStmt* expr2 = new RuleExprAsStmt(assign2);
    RuleConst* const4 = new RuleConst("8");
    RuleAssign* assign3 = new RuleAssign("x", const4);
    RuleExprAsStmt* expr3 = new RuleExprAsStmt(assign3);
    RuleIf* if1 = new RuleIf(binop1, expr2, expr3);
    
    RuleStmtConcat* root = new RuleStmtConcat(expr1, if1);
    
    vector<ComponentFunction*> compFunc;
    vector<Function*> func;
    vector<TimeDependentEC*> tdec;
    
    Environment* env = new Environment(1, 1, 1, 2, 3, compFunc, func, tdec);
    
    StateTuple* states = new StateTuple();
    root->updateEnvironment(env);
	root->updatePath();
    StateTuple statex= root->sigma(*states);
    cout << root->getStatement() << "\n\n" << endl;
	cout << "Value of x = " << statex.getPStateValue("x") << ";\n\n";*/
	
    
    RuleConst* zero = new RuleConst("0");
	RuleAssign* zIsZero = new RuleAssign("z", zero);
	RuleExprAsStmt* zStmt = new RuleExprAsStmt(zIsZero);
	RuleConst* one = new RuleConst("1");
	RuleVar* x = new RuleVar("x");
	RuleBinOp* funcAdd = new RuleBinOp("+", x, one);
	RuleAssign* funcRoot = new RuleAssign("x", funcAdd);
	RuleVar* z = new RuleVar("z");
	RuleCallF* testCall = new RuleCallF("test", z);
	RuleAssign* yIsTest = new RuleAssign("y", testCall);
	RuleFuncDef* testFunc = new RuleFuncDef("test", "x", funcRoot, yIsTest);
	RuleStmtConcat* root =  new RuleStmtConcat(zStmt, testFunc);
	
	vector<ComponentFunction*> compfuncs;
	vector<Function*> funcs;
	vector<TimeDependentEC*> tdec;
	Environment* env = new Environment(1, 2, 3, 4, 5, compfuncs, funcs, tdec);
	
	root->updateEnvironment(env);
	root->updatePath();
	
    cout << root->getStatement() << endl << endl;
    
	StateTuple* states = new StateTuple();
	StateTuple stateEnd = root->sigma(*states);
	
	cout << stateEnd.getPStateValue("y");
	
	getchar();
	return 0;
}
