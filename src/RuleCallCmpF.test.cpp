#include "libtypesystem.h"
#include <gtest/gtest.h>


class RuleCallCmpFTest: public ::testing::Test
{
	protected:
	RuleCallCmpF* testRule;
	StateTuple* startState;
	Environment* env;
	
	void SetUp()
	{
        testRule = new RuleCallCmpF("test", "testing", new RuleConst("0", new ValueInt(0)));
		vector<ComponentFunction*> compfuncs;
		vector<Function*> funcs;
		vector<TimeDependentEC*> tdec;
        TimeDependentEC* tdec1, * tdec2, * tdec3;
        tdec1 = new TimeDependentEC("test", new RuleConst("1", new ValueInt(1)));
        tdec.push_back(tdec1);
        ComponentFunction* newfunc = new ComponentFunction("test", "testing", "x", new RuleAssign( "test1", 
			new RuleConst("1", new ValueInt(1))), new RuleConst("2", new ValueInt(2)), 5);
        compfuncs.push_back(newfunc);
        env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
		testRule->updateEnvironment(env);
		startState = new StateTuple();
        startState->declareCState("test1", new ValueInt(0));
	}
	
	void TearDown(){}
};

TEST_F(RuleCallCmpFTest, Value)
{
	ValueInt* v = dynamic_cast<ValueInt*>(testRule->value(*startState));
	if(v)
		EXPECT_EQ(1, v->getValue());
	else
		FAIL();
    RuleCallCmpF* fail = new RuleCallCmpF("fail", "testing", new RuleConst("0", new ValueInt(0)));
	fail->updateEnvironment(env);
    EXPECT_THROW(fail->value(*startState), runtime_error);
}

TEST_F(RuleCallCmpFTest, State)
{
    StateTuple endState = testRule->sigma(*startState);
	ValueInt* v = dynamic_cast<ValueInt*>(endState.getCStateValue("test1"));
	if(v)
		EXPECT_EQ(1, v->getValue());   
	else
		FAIL();
}

TEST_F(RuleCallCmpFTest, Energy)
{
    EXPECT_EQ(9, testRule->energy(*startState));
}