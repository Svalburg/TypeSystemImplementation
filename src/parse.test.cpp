#include <gtest/gtest.h>
#include "parse.h"
#include "libtypesystem.h"
#include <vector>

using namespace bitpowder::lib;

namespace {

void CheckOutput(const String& expected, bool correct = true) {
    MemoryPool mp;

    auto result = ParseECAMemory(expected, mp);
    EXPECT_EQ(correct, (bool)result);
    if (result) {
        std::stringstream str;
        result.result()->print(str);
        String strOutput = str.str();
        EXPECT_EQ(expected, strOutput);
    }

}

TEST(ECAParser, AssignmentOutput) {
    CheckOutput("a:=1");
    CheckOutput("a:=a");
    CheckOutput("a:=x");
    CheckOutput("a:=:=", false);
}

TEST(ECAParser, VariableOutput) {
    CheckOutput("a");
    CheckOutput("@ab", false);
    CheckOutput("ab_123");
    CheckOutput("ab_123_ab");
    CheckOutput("123_a", false);
}

TEST(ECAParser, FaultyFunction)
{
	//this should fail because the function is not an expression
	MemoryPool mp;
	auto result = ParseECAMemory("function test(x) begin if x then skip else y := 5 end end y", mp);
	EXPECT_EQ(false, (bool)result);
	//this should fail because of the missing 'begin'
	MemoryPool mp2;
	auto result2 = ParseECAMemory("function test(x) if x then y:= 8 else y := 5 end; y end y", mp2);
	EXPECT_EQ(false, (bool)result2);
}

TEST(ECAParser, EndToEnd)
{
    /*The example from the paper, with 
     n * (i + (t +s) * U)
    n = 4
    i = 2
    t = 3
    s = 10
    u = 5
     */
    MemoryPool mp;
    auto result = ParseECAMemory("SoundSystem.on(0); repeat 4 begin SoundSystem.playBeepAtHz(5); System.sleep(0) end; SoundSystem.off(0)", mp);
    Rule* root = result.result()->getTypeRule();
    
    
    vector<ComponentFunction*> compfuncs;
    ComponentFunction* soundsystemon = new ComponentFunction("SoundSystem", "on", "zero", new RuleAssign("on", new RuleConst("1", new ValueInt(1))), 
                            new RuleConst("0", new ValueInt(0)), 0);
    compfuncs.push_back(soundsystemon);
    ComponentFunction* soundsystemoff = new ComponentFunction("SoundSystem", "off", "zero", new RuleAssign("on", new RuleConst("0", new ValueInt(0))), 
                            new RuleConst("0", new ValueInt(0)), 0);
    compfuncs.push_back(soundsystemoff);
    ComponentFunction* soundsystembeeps = new ComponentFunction("SoundSystem", "playBeepAtHz", "hz", new RuleConst("0", new ValueInt(0)), 
                            new RuleConst("2", new ValueInt(2)), 3);
    compfuncs.push_back(soundsystembeeps);
    ComponentFunction* systemsleep = new ComponentFunction("System", "sleep", "zero", new RuleConst("0", new ValueInt(0)), 
                            new RuleConst("0", new ValueInt(0)), 10);
    compfuncs.push_back(systemsleep);
	vector<Function*> funcs;
	vector<TimeDependentEC*> tdec;
    TimeDependentEC* soundsystem = new TimeDependentEC("SoundSystem", new RuleBinOp("*", new RuleConst("5", new ValueInt(5)), new RuleVar("on")));
    tdec.push_back(soundsystem);
	Environment* env = new Environment(0, 0, 0, 0, 0, 0, 0, compfuncs, funcs, tdec);
    
    StateTuple* startstate = new StateTuple();
    startstate->declareCState("on", new ValueInt(0));
    
    
	root->updateEnvironment(env);
	root->updatePath();
    
    EXPECT_EQ(268, root->energy(*startstate));
    
}

}
