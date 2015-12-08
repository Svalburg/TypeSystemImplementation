#include "libtypesystem.h"
#include <gtest/gtest.h>


class FuncDefTest: public ::testing::Test
{
	protected:
		RuleFuncDef* testRule;
		RuleFuncDef* testRuleRecur;
		StateTuple* startState;
		Environment* env;
		
		void SetUp()
		{
			testRule = new RuleFuncDef("Test", "x", new RuleConst("5"), new RuleAssign("y", new RuleCallF("Test", new RuleConst("0"))));
			testRuleRecur = new RuleFuncDef("Test", "x", 
				new RuleIf(new RuleVar("x"), new RuleVar("x"), new RuleCallF("Test", new RuleConst("100"))),
				new RuleSkip());
			vector<ComponentFunction*> compfuncs;
			vector<Function*> funcs;
			vector<TimeDependentEC*> tdec;
			Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
			testRule->updateEnvironment(env);
			testRuleRecur->updateEnvironment(env);
			startState = new StateTuple();
		}
		
		void TearDown() {}
};

TEST_F(FuncDefTest, Value)
{
	EXPECT_THROW(testRule->value(*startState), runtime_error);
}

TEST_F(FuncDefTest, State)
{
	StateTuple endState = testRule->sigma(*startState);
	EXPECT_EQ(0, endState.getCState().size());
	EXPECT_EQ(1, endState.getPState().size());
}

TEST_F(FuncDefTest, Energy)
{
	EXPECT_EQ(0, testRule->energy(*startState));
}

TEST_F(FuncDefTest, UpdateEnv)
{
	EXPECT_NO_THROW(testRule->sigma(*startState));
	EXPECT_NO_THROW(testRuleRecur->sigma(*startState));
}