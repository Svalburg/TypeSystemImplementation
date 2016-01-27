#include "libtypesystem.h"
#include <gtest/gtest.h>

class IfTest: public ::testing::Test
{
	protected:
		RuleIf* testRule;
		StateTuple* startState;
		
		void SetUp()
		{
			testRule = new RuleIf(new RuleVar("x"), new RuleAssign("y", new RuleCallCmpF("test", "0", new RuleConst("0", new ValueInt(0)))), 
			new RuleAssign("y", new RuleCallCmpF("test", "1", new RuleConst("0", new ValueInt(0)))));
			startState = new StateTuple();
			startState->declarePState("x", new ValueInt(0));
			vector<ComponentFunction*> compfuncs;
			vector<Function*> funcs;
			vector<TimeDependentEC*> tdec;
			ComponentFunction* cfunc0 = new ComponentFunction("test", "0", "z", new RuleConst("0", new ValueInt(0)), new RuleConst("0", new ValueInt(0)), 5);
			ComponentFunction* cfunc1 = new ComponentFunction("test", "1", "z", new RuleConst("1", new ValueInt(1)), new RuleConst("1", new ValueInt(1)), 5);
			compfuncs.push_back(cfunc0); compfuncs.push_back(cfunc1);
			Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
			testRule->updateEnvironment(env);
		}
		
		void TearDown() {}
};

TEST_F(IfTest, Value)
{
	EXPECT_THROW(testRule->value(*startState), runtime_error);
}

TEST_F(IfTest, State)
{
	StateTuple endState = testRule->sigma(*startState);
	ValueInt* v1 = dynamic_cast<ValueInt*>(endState.getPStateValue("y"));
	if(v1)
		EXPECT_EQ(1, v1->getValue());
	else
		FAIL();
	startState->declarePState("x", new ValueInt(1));
	endState = testRule->sigma(*startState);
	ValueInt* v2 = dynamic_cast<ValueInt*>(endState.getPStateValue("y"));
	if(v2)
		EXPECT_EQ(0, v2->getValue());
	else
		FAIL();
}

TEST_F(IfTest, Energy)
{
	EXPECT_EQ(1, testRule->energy(*startState));
	startState->declarePState("x", new ValueInt(1));
	EXPECT_EQ(0, testRule->energy(*startState));
}