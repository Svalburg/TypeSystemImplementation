#include "libtypesystem.h"
#include <gtest/gtest.h>


class EnvironmentTest: public ::testing::Test
{
    protected:
        Environment* testEnv, *testEnv2;
        
        void SetUp()
        {
            vector<ComponentFunction*> testCompFuncs, testCompFuncs2;
			vector<Function*> testFuncs, testFuncs2;
			vector<TimeDependentEC*> testTdecs, testTdecs2;
            testEnv = new Environment(1, 2, 3, 4, 5, 6, 7, testCompFuncs, testFuncs, testTdecs);
            
            RuleConst* testConst = new RuleConst("0");
            RuleConst* testConst2 = new RuleConst("1");
            Function* testFunction = new Function("x", "y", testConst);
            ComponentFunction* testComponent = new ComponentFunction("a", "z", "d", testConst, testConst2, 1);
            TimeDependentEC* testTimeD = new TimeDependentEC("b", testConst2);
			testCompFuncs2.push_back(testComponent);
			testFuncs2.push_back(testFunction);
			testTdecs2.push_back(testTimeD);
            testEnv2 = new Environment(2, 3, 4, 5, 6, 7, 8, testCompFuncs2, testFuncs2, testTdecs2);
        }
        
};

TEST_F(EnvironmentTest, Search)
{
    EXPECT_THROW(testEnv->getFunction("x"), runtime_error);
    EXPECT_THROW(testEnv->getComponentFunction("x", "y"), runtime_error);
    EXPECT_THROW(testEnv->getTimeDependentEC("x"), runtime_error);
    RuleConst* check = new RuleConst("0");
    Function* newfunc = testEnv2->getFunction("x");
    EXPECT_EQ("x", newfunc->getName());
    ComponentFunction* newcompfunc = testEnv2->getComponentFunction("a", "z");
    EXPECT_EQ("a", newcompfunc->getComponent());
    EXPECT_EQ("z", newcompfunc->getName());
    TimeDependentEC* newtdec = testEnv2->getTimeDependentEC("b");
    EXPECT_EQ("b", newtdec->getComponentState());
}

TEST_F(EnvironmentTest, Clone)
{
    Environment* clone = testEnv2->clone();
    EXPECT_EQ(clone->getTAssign(), testEnv2->getTAssign());
}

TEST_F(EnvironmentTest, Add)
{
    RuleConst* testadd = new RuleConst("7");
    testEnv->addFunction("newfunction", "x", testadd);
    EXPECT_NO_THROW(testEnv->getFunction("newfunction"));
}