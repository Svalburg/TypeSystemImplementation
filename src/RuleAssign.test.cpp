#include "libtypesystem.h"
#include <gtest/gtest.h>


class RuleAssignTest: public ::testing::Test
{
	protected:
	RuleAssign* testRule, * testRuleExpr;
	StateTuple* startState;
	
	void SetUp()
	{
		testRule = new RuleAssign("x", new RuleConst("3", new ValueInt(3)));
        testRuleExpr = new RuleAssign("y", new RuleBinOp("*", new RuleConst("3", new ValueInt(3)), new RuleConst("4", new ValueInt(4))));
		vector<ComponentFunction*> compfuncs;
		vector<Function*> funcs;
		vector<TimeDependentEC*> tdec;
        TimeDependentEC* tdec1;
        tdec1 = new TimeDependentEC("test1", new RuleConst("1", new ValueInt(1)));
        tdec.push_back(tdec1);
        Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
		testRule->updateEnvironment(env);
        testRuleExpr->updateEnvironment(env);
		startState = new StateTuple();
        startState->declareCState("test1", new ValueInt(1));
	}
	
	void TearDown(){}
};

TEST_F(RuleAssignTest, Value)
{
	ValueInt* v1 = dynamic_cast<ValueInt*>(testRule->value(*startState));
	ValueInt* v2 = dynamic_cast<ValueInt*>(testRuleExpr->value(*startState));
	if(v1)
		EXPECT_EQ(3, v1->getValue());
	else
		FAIL();
	if(v2)
		EXPECT_EQ(12, v2->getValue());
	else
		FAIL();
}

TEST_F(RuleAssignTest, State)
{
    StateTuple endState = testRule->sigma(*startState);
    StateTuple endStateExpr = testRuleExpr->sigma(*startState);
	ValueInt* v1 = dynamic_cast<ValueInt*>(endState.getPStateValue("x"));
	ValueInt* v2 = dynamic_cast<ValueInt*>(endStateExpr.getPStateValue("y"));
	if(v1)
		EXPECT_EQ(3, v1->getValue());
	else
		FAIL();
	if(v2)
		EXPECT_EQ(12, v2->getValue());
	else
		FAIL();
    EXPECT_THROW(endState.getPStateValue("y"), runtime_error);
}