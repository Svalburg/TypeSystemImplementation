#include "RuleExprConcat.cpp"
#include "RuleSkip.cpp"
#include "RuleAssign.cpp"
#include "RuleConst.cpp"
#include "Rule.cpp"
#include "Environment.cpp"
#include "TimeDependentEC.cpp"
#include "ComponentFunction.cpp"
#include "Function.cpp"
#include <gtest/gtest.h>

class ExprConcatTest: public ::testing::Test
{
	protected:
	RuleExprConcat* testRule;
	StateTuple* startState;
	
	void SetUp()
	{
		testRule = new RuleExprConcat(new RuleSkip(), new RuleAssign("x", new RuleConst("5")));
		vector<ComponentFunction*> compfuncs;
		vector<Function*> funcs;
		vector<TimeDependentEC*> tdec;
		Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
		testRule->updateEnvironment(env);
		startState = new StateTuple();
	}
	
	void TearDown(){}
};

TEST_F(ExprConcatTest, Value)
{
	EXPECT_EQ(5, testRule->value(*startState));
}

TEST_F(ExprConcatTest, Energy)
{
	EXPECT_EQ(0, testRule->energy(*startState));
}

TEST_F(ExprConcatTest, State)
{
	StateTuple endState = testRule->sigma(*startState);
	EXPECT_EQ(5, endState.getPStateValue("x"));
}