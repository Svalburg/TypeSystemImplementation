#include "libtypesystem.h"
#include <gtest/gtest.h>

class RepeatTest: public ::testing::Test
{
	protected:
		RuleRepeat* testRuleState, *testRuleEnergy;
		StateTuple* startState;
		
		void SetUp()
		{
			testRuleState = new RuleRepeat(new RuleVar("x"), new RuleAssign("x", new RuleBinOp("+", new RuleVar("x"), new RuleConst("1", new ValueInt(1)))));
			testRuleEnergy = new RuleRepeat(new RuleConst("5", new ValueInt(5)), new RuleCallCmpF("test", "testing", new RuleConst("0", new ValueInt(0))));
			vector<ComponentFunction*> compfuncs;
			vector<Function*> funcs;
			vector<TimeDependentEC*> tdec;
			compfuncs.push_back(new ComponentFunction("test", "testing", "a", new RuleSkip(), new RuleConst("5", new ValueInt(5)), 5));
			Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
			testRuleState->updateEnvironment(env);
			testRuleEnergy->updateEnvironment(env);
			startState = new StateTuple();
			startState->declarePState("x", new ValueInt(5));
		}
		
		void TearDown() {}
};

TEST_F(RepeatTest, Value)
{
	EXPECT_THROW(testRuleState->value(*startState), runtime_error);
}

TEST_F(RepeatTest, State)
{
	StateTuple endState = testRuleState->sigma(*startState);
	ValueInt* v = dynamic_cast<ValueInt*>(endState.getPStateValue("x"));
	if(v)
		EXPECT_EQ(10, v->getValue());
	else
		FAIL();
}

TEST_F(RepeatTest, Energy)
{
	EXPECT_EQ(25, testRuleEnergy->energy(*startState));
}