#include <gtest/gtest.h>
#include "parse.h"

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
}
