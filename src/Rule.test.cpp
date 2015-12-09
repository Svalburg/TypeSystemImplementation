#include "libtypesystem.h"
#include <gtest/gtest.h>


class RuleTest: public ::testing::Test
{
	protected:
	Rule* testRule;
	StateTuple* startState;
	
	void SetUp()
	{
		testRule = new Rule();
		vector<ComponentFunction*> compfuncs;
		vector<Function*> funcs;
		vector<TimeDependentEC*> tdec;
        TimeDependentEC* tdec1, * tdec2, * tdec3;
        tdec1 = new TimeDependentEC("test1", new RuleConst("1"));
        tdec2 = new TimeDependentEC("test2", new RuleConst("2"));
        tdec3 = new TimeDependentEC("test3", new RuleConst("3"));
        tdec.push_back(tdec1); tdec.push_back(tdec2); tdec.push_back(tdec3);
        Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
		testRule->updateEnvironment(env);
		startState = new StateTuple();
        startState.declareCState("test1", 1);
        startState.declareCState("test2", 2);
        startState.declareCState("test3", 3);
	}
	
	void TearDown(){}
};

TEST_F(RuleTest, tdec)
{
    EXPECT_EQ(6, testRule->tdec());
}