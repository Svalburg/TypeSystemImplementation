#include "libtypesystem.h"
#include <gtest/gtest.h>


class RuleAssignTest: public ::testing::Test
{
	protected:
	RuleAssign* testRule, * testRuleExpr;
	StateTuple* startState;
	
	void SetUp()
	{
		testRule = new RuleAssign("x", new RuleConst("3"));
        testRuleExpr = new RuleAssign("y", new RuleBinOp("*", new RuleConst("3"), new RuleConst("4")));
		vector<ComponentFunction*> compfuncs;
		vector<Function*> funcs;
		vector<TimeDependentEC*> tdec;
        TimeDependentEC* tdec1;
        tdec1 = new TimeDependentEC("test1", new RuleConst("1"));
        tdec.push_back(tdec1);
        Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
		testRule->updateEnvironment(env);
        testRuleExpr->updateEnvironment(env);
		startState = new StateTuple();
        startState.declareCState("test1", 1);
	}
	
	void TearDown(){}
};

TEST_F(RuleAssignTest, Value)
{
    EXPECT_EQ(3, testRule->value());
    EXPECT_EQ(12, testRuleExpr->value());
}

TEST_F(RuleAssignTest, State)
{
    StateTuple* endState = testRule->sigma(*startState);
    StateTuple* endStateExpr = testRuleExpr->(*startState);
    EXPECT_EQ(3, endState.getPStateValue("x"));
    EXPECT_EQ(12, endStateExpr.getPStateValue("y"));
    EXPECT_THROW(endState.getPStateValue("y"), runtime_error);
}

TEST_F(RuleAssignTest, Energy)
{
    EXPECT_EQ(6, testRule->energy());
    EXPECT_EQ(9, testRuleExpr->energy());
}