#include "libtypesystem.h"
#include <gtest/gtest.h>


class RuleBinOpTest: public ::testing::Test
{
	protected:
	RuleBinOp* testRule;
	StateTuple* startState;
	Environment* env;
	
	void SetUp()
	{
        testRule = new RuleBinOp("*", new RuleConst("2", new ValueInt(2)), new RuleConst("3", new ValueInt(3)));
		vector<ComponentFunction*> compfuncs;
		vector<Function*> funcs;
		vector<TimeDependentEC*> tdec;
        TimeDependentEC* tdec1, * tdec2, * tdec3;
        tdec1 = new TimeDependentEC("test1", new RuleConst("1", new ValueInt(1)));
        tdec.push_back(tdec1);
        env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
		testRule->updateEnvironment(env);
		startState = new StateTuple();
        startState->declareCState("test1", new ValueInt(1));
	}
	
	void TearDown(){}
};

TEST_F(RuleBinOpTest, Value)
{
    RuleBinOp* fail = new RuleBinOp("cfaaga", new RuleConst("1", new ValueInt(1)), new RuleConst("1", new ValueInt(1)));
	fail->updateEnvironment(env);
    EXPECT_THROW(fail->value(*startState), runtime_error);
	ValueInt* v = dynamic_cast<ValueInt*>(testRule->value(*startState));
	if(v)
		EXPECT_EQ(6, v->getValue());       
	else
		FAIL();
}