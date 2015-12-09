#include "libtypesystem.h"
#include <gtest/gtest.h>


class RuleBinOpTest: public ::testing::Test
{
	protected:
	RuleBinop* testRule;
	StateTuple* startState;
	
	void SetUp()
	{
        testRule = new RuleBinOp("*", new RuleConst("2"), new RuleConst("3"));
		vector<ComponentFunction*> compfuncs;
		vector<Function*> funcs;
		vector<TimeDependentEC*> tdec;
        TimeDependentEC* tdec1, * tdec2, * tdec3;
        tdec1 = new TimeDependentEC("test1", new RuleConst("1"));
        tdec.push_back(tdec1);
        Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
		testRule->updateEnvironment(env);
		startState = new StateTuple();
        startState.declareCState("test1", 1);
	}
	
	void TearDown(){}
};

TEST_F(RuleBinOpTest, Value)
{
    RuleBinOp* fail = new RuleBinOp("cfaaga", new RuleConst("1"), new RuleConst("1"));
    EXPECT_THROW(fail->value(*startState), runtime_error);
    EXPECT_EQ(6, testRule->value(*startState));       
}