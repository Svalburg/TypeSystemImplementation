/**
Copyright 2010-2015 Bernard van Gastel, bvgastel@bitpowder.com.
This file is part of Bit Powder Libraries.

Bit Powder Libraries is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Bit Powder Libraries is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Bit Powder Libraries.  If not, see <http://www.gnu.org/licenses/>.
*/

#define DEBUG_EXCEPTIONS

#include "stringparse.h"
#include <functional>
#include <iterator>
#include <unistd.h>  // NOLINT
#include <gtest/gtest.h>

#include <map>
#include <string>
#include <ostream>
#include <iostream>

#include <math.h>


namespace tests {
using namespace bitpowder::lib;

TEST(StringParse, Accept) {
    EXPECT_TRUE(StringParser("Hello Hello").accept("Hello").span(Char<' '>()).accept("Hello").empty());
}

TEST(StringParse, Number) {
    int parsedInt = -1;
    EXPECT_TRUE(StringParser("number=0;something").accept("number=").parseNumber(parsedInt).accept(";something").empty());
    EXPECT_EQ(parsedInt, 0);
    // try to parse a letter
    EXPECT_FALSE(StringParser("number=a").accept("number=").parseNumber(parsedInt).empty());
    // on failure, the int is not modified
    EXPECT_EQ(parsedInt, 0);
}

TEST(StringParse, Opt) {
    auto c = [](const String &str) -> bool {
        int errorCode;
        int errorFile;
        return StringParser(str).accept("FAILED[").parseNumber(errorCode).opt([&errorFile](StringParser& cont) {
            return cont.accept(":").parseNumber(errorFile);
        }).accept("]");
    };
    EXPECT_TRUE(c("FAILED[-1:1]"));
    EXPECT_TRUE(c("FAILED[-1]"));
    EXPECT_FALSE(c("FAILED[-1"));
    EXPECT_FALSE(c("FAILED[-1:lala]"));
}

int constantValue = 0;

StringParser& constant(StringParser& cont) {
    return cont.parseNumber(constantValue);
}

StringParser& expr(StringParser& start);

StringParser& haakjes(StringParser& cont) {
    return cont.accept("(").perform(expr).accept(")");
}

StringParser& primary(StringParser& cont) {
    return cont.choose(haakjes, constant);
}

StringParser& mulTail(StringParser& cont) {
    int a = constantValue;
    if (cont.accept("*").perform(primary)) {
        constantValue *= a;
    }
    return cont.opt(mulTail);
}

StringParser& mul(StringParser& cont) {
    return cont.perform(primary).opt(mulTail);
}

StringParser& plusTail(StringParser& cont) {
    int a = constantValue;
    if (cont.accept("+").perform(mul)) {
        constantValue += a;
    }
    return cont.opt(plusTail);
}

StringParser& plus(StringParser& cont) {
    return cont.perform(mul).opt(plusTail);
}

StringParser& expr(StringParser& start) {
    return plus(start);
}

TEST(StringParse, CalculatorOfPositiveValues) {
    EXPECT_TRUE(StringParser("2+1").opt(expr).empty());
    EXPECT_EQ(constantValue, 3);
    EXPECT_TRUE(StringParser("3*3+2*2+1*1").opt(expr).empty());
    EXPECT_EQ(constantValue, 14);
    EXPECT_TRUE(StringParser("(3*3)+(2*2)+(1*1)").opt(expr).empty());
    EXPECT_EQ(constantValue, 14);
    EXPECT_TRUE(StringParser("3*(3+2)*(2+1)*1").opt(expr).empty());
    EXPECT_EQ(constantValue, 45);
}

}
