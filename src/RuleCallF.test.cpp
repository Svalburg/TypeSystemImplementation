#include "libtypesystem.h"
#include <gtest/gtest.h>


class RuleCallFTest: public ::testing::Test
{
	protected:
	RuleFuncDef* testRule, * testRecursion;
	StateTuple* startState;
	
	void SetUp()
	{
        //Rule:  y := testing(0)
        RuleAssign* test = new RuleAssign( "y", new RuleCallF("testing", new RuleConst("0", new ValueInt(0)))); 
        //Rule:  z := recursion(0)
        RuleAssign* recursive = new RuleAssign( "z", new RuleCallF("recursion", new RuleConst("0", new ValueInt(0))));  
		vector<ComponentFunction*> compfuncs;
		vector<Function*> funcs;
		vector<TimeDependentEC*> tdec;
        //Add a constant energy usage of 1 to the model
        TimeDependentEC* tdec1;
        tdec1 = new TimeDependentEC("test1", new RuleConst("1", new ValueInt(1)));
        tdec.push_back(tdec1);
        //Rule: function testing(x) begin d := 1 + 2 end y := testing(0)
        testRule = new RuleFuncDef("testing", "x", 
            new RuleAssign("d", new RuleBinOp("+", new RuleConst("1", new ValueInt(1)), new RuleConst("2", new ValueInt(2)))),
            test);  
        //Rule: function recursion(x) begin if x then x := 1 else x := recursion(10), x end z := recursion(0) 
        testRecursion = new RuleFuncDef("recursion", "x", 
                new RuleExprConcat(
                    new RuleIf(new RuleVar("x"), new RuleAssign("x", new RuleConst("1", new ValueInt(1))), 
					new RuleAssign("x", new RuleCallF("recursion", new RuleConst("10", new ValueInt(10))))),new RuleVar("x")),
				recursive); 
        Environment* env = new Environment(1, 2, 3, 4, 5, 6, 7, compfuncs, funcs, tdec);
		testRule->updateEnvironment(env);
        testRecursion->updateEnvironment(env);
		startState = new StateTuple();
        //Add component variable test1 to Component State with initial value 1
        startState->declareCState("test1", new ValueInt(1));
        //Add program variables y  and z to Program State with value 0
        startState->declarePState("y", new ValueInt(0));
        startState->declarePState("z", new ValueInt(0));
	}
	
	void TearDown(){}
};

TEST_F(RuleCallFTest, Value)
{
    //Tests if a normal function returns the right value
    StateTuple endState = testRule->sigma(*startState);
	ValueInt* v1 = dynamic_cast<ValueInt*>(endState.getPStateValue("y"));
	if(v1)
		EXPECT_EQ(3, v1->getValue());
	else
		FAIL();
    //Tests if a recursive function returns the right value
    endState = testRecursion->sigma(*startState);
	ValueInt* v2 = dynamic_cast<ValueInt*>(endState.getPStateValue("z"));
	if(v2)
		EXPECT_EQ(1, v2->getValue());
	else
		FAIL();
}

TEST_F(RuleCallFTest, Sigma)
{
    //Tests if a normal function does not add variables out of scope
    StateTuple endState = testRule->sigma(*startState);
    EXPECT_EQ(2, endState.getPState().size());
    //Tests if a recursive function does not add variables out of scope
    endState = testRecursion->sigma(*startState);
    EXPECT_EQ(2, endState.getPState().size());    
}