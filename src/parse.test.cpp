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

}
