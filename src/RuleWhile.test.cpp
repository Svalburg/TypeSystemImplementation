#include "libtypesystem.h"
#include <gtest/gtest.h>


class WhileTest: public ::testing::Test
{
	protected:
		RuleWhile* testRule;
		StateTuple* startState;
		
		void StartUp()
		{
			RuleCallCmpF* rule1 = new RuleCallCmpF("test", "testing", new RuleConst("0"));
			RuleAssign* rule2 = new RuleAssign("x", new RuleBinOp("+", new RuleVar("x"), new RuleConst("1")));
			testRule = new RuleWhile(new RuleBinOp("<", new RuleVar("x"), new RuleConst("10")), new RuleStmtConcat(rule1, rule2));
			vector<ComponentFunction*> compfuncs;
			vector<Function*> funcs;
			vector<TimeDependentEC*> tdec;
			compfuncs.push_back(new ComponentFunction("test", "testing", "a", new RuleSkip(), new RuleConst("5"), 5));
			Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
			testRule->updateEnvironment(env);
			startState = new StateTuple();
			startState->declarePState("x", 5);
		}
		
		void TearDown() {}
};

TEST_F(WhileTest, Value)
{
	EXPECT_THROW(testRule->value(*startState), runtime_error);
}

TEST_F(WhileTest, State)
{
	//StateTuple endState = testRule->sigma(*startState);
	//EXPECT_EQ(10, endState.getPStateValue("x"));
}

TEST_F(WhileTest, Energy)
{
	//EXPECT_EQ(25, testRule->energy(*startState));
}