#include "libtypesystem.h"
#include <gtest/gtest.h>


class RuleTest: public ::testing::Test
{
	protected:
	RuleConst* testRule;
	StateTuple* startState;
	
	void SetUp()
	{
		testRule = new RuleConst("0", new ValueInt(0));
		vector<ComponentFunction*> compfuncs;
		vector<Function*> funcs;
		vector<TimeDependentEC*> tdec;
        TimeDependentEC* tdec1, * tdec2, * tdec3;
		ComponentFunction *func1, *func2, *func3;
		func1 = new ComponentFunction("test1", "test", "x", new RuleSkip(), new RuleConst("0", new ValueInt(0)), 1);
		func2 = new ComponentFunction("test2", "test", "x", new RuleSkip(), new RuleConst("0", new ValueInt(0)), 1);
		func3 = new ComponentFunction("test3", "test", "x", new RuleSkip(), new RuleConst("0", new ValueInt(0)), 1);
        tdec1 = new TimeDependentEC("test1", new RuleConst("1", new ValueInt(1)));
        tdec2 = new TimeDependentEC("test2", new RuleConst("2", new ValueInt(2)));
        tdec3 = new TimeDependentEC("test3", new RuleConst("3", new ValueInt(3)));
        tdec.push_back(tdec1); tdec.push_back(tdec2); tdec.push_back(tdec3);
		compfuncs.push_back(func1); compfuncs.push_back(func2); compfuncs.push_back(func3);
        Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
		testRule->updateEnvironment(env);
		startState = new StateTuple();
	}
	
	void TearDown(){}
};

TEST_F(RuleTest, tdec)
{
    EXPECT_EQ(12, testRule->energy(*startState));
}