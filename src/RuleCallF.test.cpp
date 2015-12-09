#include "libtypesystem.h"
#include <gtest/gtest.h>


class RuleCallFTest: public ::testing::Test
{
	protected:
	RuleCallF* testRule;
	StateTuple* startState;
	
	void SetUp()
	{
        testRule = new RuleCallF("testing", new RuleConst("0"));
		vector<ComponentFunction*> compfuncs;
		vector<Function*> funcs;
		vector<TimeDependentEC*> tdec;
        TimeDependentEC* tdec1, * tdec2, * tdec3;
        tdec1 = new TimeDependentEC("test1", new RuleConst("1"));
        tdec.push_back(tdec1);
        Function* funcdef = new Function("testing", "x", 
            new RuleAssign("y", new RuleBinOp("+", new RuleConst("1"), new RuleConst("2"))));
        funcs.push_back(funcdef);
        Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
		testRule->updateEnvironment(env);
		startState = new StateTuple();
        startState->declareCState("test1", 0);
	}
	
	void TearDown(){}
};

TEST_F(RuleCallFTest, Value)
{
    EXPECT_EQ(3, testRule->value(*startState));
}
