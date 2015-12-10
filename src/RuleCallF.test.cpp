#include "libtypesystem.h"
#include <gtest/gtest.h>


class RuleCallFTest: public ::testing::Test
{
	protected:
	RuleFuncDef* testRule, * testRecursion;
	StateTuple* startState;
	
	void SetUp()
	{
        RuleCallF* test = new RuleCallF("testing", new RuleConst("0"));
        RuleCallF* recursive = new RuleCallF("recursion", new RuleConst("0"));
		vector<ComponentFunction*> compfuncs;
		vector<Function*> funcs;
		vector<TimeDependentEC*> tdec;
        TimeDependentEC* tdec1, * tdec2, * tdec3;
        tdec1 = new TimeDependentEC("test1", new RuleConst("1"));
        tdec.push_back(tdec1);
        RuleFuncDef* funcdef = new RuleFuncDef("testing", "x", 
            new RuleAssign("y", new RuleBinOp("+", new RuleConst("1"), new RuleConst("2"))),
            test);
        RuleFuncDef* testRecursion = new RuleFuncDef("recursion", "x", 
				new RuleIf(new RuleVar("x"), new RuleVar("x"), new RuleCallF("recursion", new RuleConst("100"))),
				testRecursion);
        Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
		testRule->updateEnvironment(env);
        //testRecursion->updateEnvironment(env);
		startState = new StateTuple();
        startState->declareCState("test1", 1);
        startState->declarePState("x", 0);
	}
	
	void TearDown(){}
};

TEST_F(RuleCallFTest, Value)
{
    //EXPECT_EQ(3, testRule->value(*startState));
    //StateTuple endState = testRecursion->sigma(*startState);
    //EXPECT_EQ(2, endState.getPStateValue("x"));
}

TEST_F(RuleCallFTest, Sigma)
{
    StateTuple endState = testRule->sigma(*startState);
    EXPECT_EQ(3, endState.getPStateValue("y"));
}