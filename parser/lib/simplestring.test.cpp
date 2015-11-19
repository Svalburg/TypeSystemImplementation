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

#include <unistd.h>  // NOLINT
#include <gtest/gtest.h>
#include "simplestring.h"
#include "rawreader.h"

namespace {
using namespace bitpowder::lib;

TEST(String, BoolConversion) {
    StaticMemoryPool<64> mp;
    String t = ("test="_S + true)(mp);
    EXPECT_EQ("test=true"_S, t);
    String f = ("test="_S + false)(mp);
    EXPECT_EQ("test=false"_S, f);
}

template <class T>
void testStringConversionFor(T i, const char* expected, int p = 1) {
    StaticMemoryPool<64> mp;
    if (p > 1) {
        StringT test = ("test-"_S + precision(p) + i).c_str(mp);
        if (test != String(expected))
            fprintf(stderr, "%s == '%s'\n", expected, test.c_str());
        EXPECT_EQ(test,String(expected));
    } else {
        StringT test = ("test-"_S + i).c_str(mp);
        if (test != String(expected))
            fprintf(stderr, "%s == '%s'\n", expected, test.c_str());
        EXPECT_EQ(test,String(expected));
    }
}

TEST(String, NumberConvertionDecimal) {
    testStringConversionFor<int>(0, "test-0");
    testStringConversionFor<int>(-1, "test--1");
    testStringConversionFor<int>(0xFFFFFFFF, "test--1");
    testStringConversionFor<int>(-10, "test--10");
    testStringConversionFor<int>(10, "test-10");
    testStringConversionFor<int>(0x7FFFFFFF, "test-2147483647");
    testStringConversionFor<int>(-0x80000000, "test--2147483648");
    testStringConversionFor<unsigned int>(0, "test-0");
    testStringConversionFor<unsigned int>(0, "test-00000", 5);
    testStringConversionFor<unsigned int>(10, "test-10");
    testStringConversionFor<unsigned int>(0x7FFFFFFF, "test-2147483647");
    testStringConversionFor<unsigned int>(-0x80000000, "test-2147483648");
    testStringConversionFor<unsigned int>(0xFFFFFFFF, "test-4294967295");
    testStringConversionFor<unsigned int>(-1, "test-4294967295");
}

template <class T>
void testHexStringConversionFor(T i, const char* expected, int p = 1) {
    StaticMemoryPool<64> mp;
    if (p > 1) {
        StringT test = ("test-"_S + ToHexString(i, p)).c_str(mp);
        if (test != String(expected))
            fprintf(stderr, "%s == '%s'\n", expected, test.c_str());
        EXPECT_EQ(test,String(expected));
    } else {
        StringT test = ("test-"_S + ToHexString(i)).c_str(mp);
        if (test != String(expected))
            fprintf(stderr, "%s == '%s'\n", expected, test.c_str());
        EXPECT_TRUE(test == String(expected));
    }
}

TEST(String, NumberConvertionHex) {
    testHexStringConversionFor<int>(0, "test-0");
    testHexStringConversionFor<int>(-1, "test--1");
    testHexStringConversionFor<int>(0xFFFFFFFF, "test--1");
    testHexStringConversionFor<int>(-10, "test--a");
    testHexStringConversionFor<int>(10, "test-a");
    testHexStringConversionFor<int>(0x7FFFFFFF, "test-7fffffff");
    testHexStringConversionFor<int>(-0x80000000, "test--80000000");
    testHexStringConversionFor<unsigned int>(0, "test-0");
    testHexStringConversionFor<unsigned int>(0, "test-00000", 5);
    testHexStringConversionFor<unsigned int>(10, "test-a");
    testHexStringConversionFor<unsigned int>(0x7FFFFFFF, "test-7fffffff");
    testHexStringConversionFor<unsigned int>(-0x80000000, "test-80000000");
    testHexStringConversionFor<unsigned int>(0xFFFFFFFF, "test-ffffffff");
    testHexStringConversionFor<unsigned int>(-1, "test-ffffffff");
}

TEST(String, PathStuff) {
    String test = "/test/bla/test.ext/bla";
    EXPECT_EQ("bla"_S, test.base());
    EXPECT_EQ(""_S, test.extension());
    EXPECT_EQ("/test/bla/test.ext"_S, test.dir());
    EXPECT_EQ("ext"_S, test.dir().extension());
    String test2 = "somefile";
    EXPECT_EQ(".", test2.dir());
    EXPECT_EQ(test2, test2.base());
    EXPECT_EQ("somefile", test2.base());
    EXPECT_EQ("", test2.extension());
}

TEST(String, ToNumber) {
    EXPECT_EQ(10, "10"_S.toNumber<int>());
    EXPECT_EQ(100, "100"_S.toNumber<int>());
    EXPECT_EQ(101, "101"_S.toNumber<int>());
    EXPECT_EQ(1012, "1012"_S.toNumber<int>());
    EXPECT_EQ(-1012, "-1012"_S.toNumber<int>());

    EXPECT_FLOAT_EQ(.1234, ".1234"_S.toNumber<float>());
    EXPECT_FLOAT_EQ(-.1234, "-.1234"_S.toNumber<float>());
    EXPECT_FLOAT_EQ(10.0, "10"_S.toNumber<float>());
    EXPECT_FLOAT_EQ(10.11f, "10.11"_S.toNumber<float>());
    EXPECT_FLOAT_EQ(100.99999f, "100.99999"_S.toNumber<float>());
    EXPECT_FLOAT_EQ(101.1234567890f, "101.1234567890"_S.toNumber<float>()); // to large number for float
    EXPECT_FLOAT_EQ(1012.1111111f, "1012.1111111"_S.toNumber<float>()); // // to large number for float
    EXPECT_FLOAT_EQ(101.1234567890, "101.1234567890"_S.toNumber<double>());
    EXPECT_FLOAT_EQ(1012.1111111, "1012.1111111"_S.toNumber<double>());
    EXPECT_FLOAT_EQ(-1012.111f, "-1012.111"_S.toNumber<float>());

    String remainder;
    EXPECT_FLOAT_EQ(.1234e0, ".1234E0test"_S.toNumber<float>(remainder));
    EXPECT_EQ("test"_S, remainder);
    EXPECT_FLOAT_EQ(-.1234e1, "-.1234e1double"_S.toNumber<float>(remainder));
    EXPECT_EQ("double"_S, remainder);
    EXPECT_FLOAT_EQ(10.0e2, "10E2some"_S.toNumber<float>(remainder));
    EXPECT_EQ("some"_S, remainder);
    EXPECT_FLOAT_EQ(10.11e3f, "10.11E3"_S.toNumber<float>(remainder));
    EXPECT_EQ(""_S, remainder);
    EXPECT_FLOAT_EQ(100.99999e4f, "100.99999E4"_S.toNumber<float>(remainder));
    EXPECT_EQ(""_S, remainder);
    EXPECT_FLOAT_EQ(101.1234567890e5f, "101.1234567890E5"_S.toNumber<float>(remainder)); // to large number for float
    EXPECT_EQ(""_S, remainder);
    EXPECT_FLOAT_EQ(1012.1111111e6f, "1012.1111111E6"_S.toNumber<float>(remainder)); // // to large number for float
    EXPECT_EQ(""_S, remainder);
    EXPECT_FLOAT_EQ(101.1234567890e7, "101.1234567890E7"_S.toNumber<double>(remainder));
    EXPECT_EQ(""_S, remainder);
    EXPECT_FLOAT_EQ(1012.1111111e8, "1012.1111111E8"_S.toNumber<double>(remainder));
    EXPECT_EQ(""_S, remainder);
    EXPECT_FLOAT_EQ(-1012.111e11f, "-1012.111e11"_S.toNumber<float>(remainder));
    EXPECT_EQ(""_S, remainder);

    EXPECT_EQ(-0x80000000, "-80000000"_S.hexToNumber<int>());
    EXPECT_EQ(0x7FFFFFFF, "7fffffff"_S.hexToNumber<int>());
    EXPECT_EQ(0x7FFFFFFF, "7FFFFFFF"_S.hexToNumber<int>());
    EXPECT_EQ(0x10, "10"_S.hexToNumber<int>());
    EXPECT_EQ(0x1012, "1012"_S.hexToNumber<int>());
    EXPECT_EQ(-0x1012, "-1012"_S.hexToNumber<int>());
}

TEST(String, FloatToString) {
    MemoryPool mp;
    EXPECT_EQ("0.123399999999999"_S, ToString(.1234)(mp));
    EXPECT_EQ("-0.12339999999999"_S, ToString(-.1234)(mp));
    EXPECT_EQ("10"_S, ToString(10.0)(mp));
    EXPECT_EQ("10.10999999999999"_S, ToString(10.11)(mp));
    EXPECT_EQ("100.9999899999999"_S, ToString(100.99999)(mp));
    EXPECT_EQ("101.1234"_S, ToString(101.1234567890f)(mp)); // to large number for float
    EXPECT_EQ("1012.111"_S, ToString(1012.1111111111111f)(mp)); // to large number for float
    EXPECT_EQ("101.1234567890000"_S, ToString(101.1234567890)(mp));
    EXPECT_EQ("1012.111111111111"_S, ToString(1012.1111111111111)(mp));
    EXPECT_EQ("-1012.11"_S, ToString(-1012.111f)(mp));
}

TEST(String, Number) {
    StaticMemoryPool<64> smp;
    for (short i = std::numeric_limits<short>::min(); i < std::numeric_limits<short>::max(); ++i) {
        smp.clear();
        String str = ToString(i)(smp);
        EXPECT_EQ(i, str.toNumber<short>());
    }
}

TEST(String, HexNumber) {
    StaticMemoryPool<64> smp;
    for (short i = std::numeric_limits<short>::min(); i < std::numeric_limits<short>::max(); ++i) {
        smp.clear();
        String str = ToHexString(i)(smp);
        EXPECT_EQ(i, str.hexToNumber<short>());
    }
}

TEST(String, MaxNumber) {
    StaticMemoryPool<64> smp;
    for (short i = std::numeric_limits<short>::min(); i < std::numeric_limits<short>::max(); ++i) {
        smp.clear();
        String str = ToMaxString(i)(smp);
        EXPECT_EQ(i, str.maxToNumber<short>());
    }
}

TEST(String, EscapeURL) {
    StaticMemoryPool<128> mp;
    String escaped = "bla%20/:%3F%5D%5B%C3%AB";
    String escapedAlt = "bla%20/:%3f%5d%5b%c3%ab";
    String unescaped = "bla /:?][ë"; // because this is unicode
    EXPECT_EQ(unescaped, escaped.unescape(String::Escape::URL, mp));
    EXPECT_EQ(unescaped, escapedAlt.unescape(String::Escape::URL, mp));
    EXPECT_EQ(escaped, unescaped.escape(String::Escape::URL, mp));
}

TEST(String, EscapeLikeC) {
    StaticMemoryPool<128> mp;
    String escaped = "\\a\\b\\f\\n\\r\\t\\v\\\\\\\"abc";
    String unescaped = "\a\b\f\n\r\t\v\\\"abc";
    EXPECT_EQ(unescaped, escaped.unescape(String::Escape::LikeC, mp));
    EXPECT_EQ(escaped, unescaped.escape(String::Escape::LikeC, mp));
}

TEST(String, rspan) {
    static Char<' '> space;
    String test = "  abc  ";
    String remainder;
    String spanned = test.rspan(space, remainder);
    EXPECT_EQ(test.substring(0, 5), remainder);
    EXPECT_EQ("  abc", remainder);
    EXPECT_EQ(test.substring(5), spanned);
    EXPECT_EQ("  ", spanned);
}

TEST(String, span) {
    static Char<' '> space;
    String test = "  abc  ";
    String remainder;
    String spanned = test.span(space, remainder);
    EXPECT_EQ(test.substring(2), remainder);
    EXPECT_EQ("abc  ", remainder);
    EXPECT_EQ(test.substring(0, 2), spanned);
    EXPECT_EQ("  ", spanned);
}

TEST(String, split) {
    static Char<' '> space;
    String test = "abc def";
    String tail;
    char delim = '\0';
    String head = test.splitOn(space, tail, &delim);
    EXPECT_EQ(test.substring(0, 3), head);
    EXPECT_EQ("abc", head);
    EXPECT_EQ(test.substring(4, 3), tail);
    EXPECT_EQ("def", tail);
    EXPECT_EQ(' ', delim);
}

TEST(String, rsplit) {
    static Char<' '> space;
    String test = "abc def";
    String tail;
    char delim = '\0';
    String head = test.rsplitOn(space, tail, &delim);
    EXPECT_EQ(test.substring(4, 3), head);
    EXPECT_EQ("def", head);
    EXPECT_EQ(test.substring(0, 3), tail);
    EXPECT_EQ("abc", tail);
    EXPECT_EQ(' ', delim);
}

TEST(String, trim) {
    String test = "  a b c  ";
    String trimmed = test.trim();
    EXPECT_EQ("a b c", trimmed);
}

TEST(String, rsplit_string) {
    String test = "01234567890";
    String tail;
    String head = test.rsplit("456"_S, tail);
    EXPECT_EQ("0123", tail);
    EXPECT_EQ("7890", head);
    head = test.rsplit("890"_S, tail);
    EXPECT_EQ("01234567", tail);
    EXPECT_EQ("", head);

    tail = "foobar";
    head = test.rsplit(String(), tail);
    EXPECT_EQ("01234567890", head);
    EXPECT_EQ("", tail);
}

TEST(String, split_string) {
    String test = "01234567890";
    String tail;
    String head = test.split("456"_S, tail);
    EXPECT_EQ("0123", head);
    EXPECT_EQ("7890", tail);
    head = test.split("890"_S, tail);
    EXPECT_EQ("01234567", head);
    EXPECT_EQ("", tail);

    tail = "foobar";
    head = test.split(String(), tail);
    EXPECT_EQ("01234567890", head);
    EXPECT_EQ("", tail);
}

TEST(HashedIgnoreCaseString, hashTest) {
    HashedIgnoreCase<String> test = ""_HSIC;
    EXPECT_EQ(test.hashIgnoreCase(), test.string().hashIgnoreCase());
    test = "abc";
    EXPECT_EQ(test.hashIgnoreCase(), test.string().hashIgnoreCase());
    test = "cde";
    EXPECT_EQ(test.hashIgnoreCase(), test.string().hashIgnoreCase());
    test = "lala";
    EXPECT_EQ(test.hashIgnoreCase(), test.string().hashIgnoreCase());
    test = "SpeedHTTPd";
    EXPECT_EQ(test.hashIgnoreCase(), test.string().hashIgnoreCase());
    EXPECT_TRUE("lala"_S == String("lala"_HS));
    EXPECT_TRUE("lala"_HSIC == StableHashedIgnoreCase<String>("lala"_S));
    EXPECT_TRUE("lala"_HSIC == HashedIgnoreCase<String>("lala"_S));
    EXPECT_FALSE("laLa"_S == String("lala"_HS));
    EXPECT_FALSE("laLa"_HSIC == StableHashedIgnoreCase<String>("lala"_S));
    EXPECT_FALSE("laLa"_HSIC == HashedIgnoreCase<String>("lala"_S));
    EXPECT_TRUE("laLa"_S.equalsIgnoreCase("lala"_HSIC));
    EXPECT_TRUE("laLa"_HSIC.equalsIgnoreCase("lala"_S));
}

TEST(HashedString, hashTest) {
    Hashed<String> test = ""_HS;
    EXPECT_EQ(test.hash(), test.string().hash());
    test = "abc";
    EXPECT_EQ(test.hash(), test.string().hash());
    test = "cde";
    EXPECT_EQ(test.hash(), test.string().hash());
    test = "lala";
    EXPECT_EQ(test.hash(), test.string().hash());
    test = "SpeedHTTPd";
    EXPECT_EQ(test.hash(), test.string().hash());
    EXPECT_TRUE("lala"_S == String("lala"_HS));
    EXPECT_TRUE("lala"_HS == StableHashed<String>("lala"_S));
    EXPECT_TRUE("lala"_HS == Hashed<String>("lala"_S));
    EXPECT_FALSE("laLa"_S == String("lala"_HS));
    EXPECT_FALSE("laLa"_HS == StableHashed<String>("lala"_S));
    EXPECT_FALSE("laLa"_HS == Hashed<String>("lala"_S));
    EXPECT_FALSE("laLa"_S == "lala"_HS);
    EXPECT_FALSE("laLa"_HS == "lala"_S);
}

TEST(String, contains) {
    String test = "01234567890";
    EXPECT_TRUE(test.contains("0123"));
    EXPECT_TRUE(test.contains("456"));
    EXPECT_TRUE(test.contains("9"));
    EXPECT_TRUE(test.contains("890"));
    EXPECT_FALSE(test.contains("9012"));
    EXPECT_FALSE(test.contains("465"));
    EXPECT_FALSE(test.contains("78901"));
}

TEST(String, StringHexConversion) {
    const std::pair<String,String> tests[] = {
        {"ABCDEF"_S,"414243444546"_S},
        {"a"_S, "61"_S},
        {"\0"_S, "00"_S},
        {"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14"_S, "000102030405060708090a0b0c0d0e0f1011121314"_S},
        {"\xff\xfe"_S, "fffe"_S},
        {"\xaa"_S, "aa"_S},
        {""_S, ""_S}
    };
    for (auto& t : tests) {
        auto& testIn = t.first;
        auto& testExpectedHex = t.second;
        StaticMemoryPool<128> smp;
        String testHex = ToHexString(testIn)(smp);
        EXPECT_EQ(testExpectedHex, testHex);
        String testOut = FromHexString(testHex)(smp);
        EXPECT_EQ(testOut, testIn);
    }
}

TEST(String, ConcatSubstring) {
    StaticMemoryPool<64> mp;
    String t = ("12345"_S + "67890"_S)(mp);
    EXPECT_EQ("1234567890"_S, t);

    for (int i = 0; i < 11; ++i) {
        String t = ("12345"_S + "67890"_S)._substring(i)(mp);
        EXPECT_EQ("1234567890"_S.substring(i), t);
    }
}

TEST(String, RawString) {
    StaticMemoryPool<64> mp;
    String t = ToRawString("12345"_S)(mp);
    EXPECT_EQ("\0\0\0\00512345"_S, t);

    for (int i = 0; i < 10; ++i) {
        String t = ToRawString("12345"_S)._substring(i)(mp);
        EXPECT_EQ("\0\0\0\00512345"_S.substring(i), t) << i;
    }
}

TEST(String, RawNumber) {
    StaticMemoryPool<64> mp;
    String t = ToRawU32(0x11223344)(mp);
    EXPECT_EQ("\x11\x22\x33\x44", t);
}

TEST(String, RawSame) {
    String key = "key";
    String attributes = "attributes";
    String valueType = "valueType";
    String value = "value";
    unsigned char CMD_PUT = 1;
    std::size_t packetContentLength = 1+4+(4+key.size())+(4+attributes.size())+(4+valueType.size())+(4+value.size());
    StringContainer message(4+packetContentLength);
    RawWriter writer = {message};
    writer.writeUnsignedInt32(packetContentLength);
    writer.writeByte(CMD_PUT);
    writer.writeUnsignedInt32(0); // store request
    writer.writeString(key);
    writer.writeString(attributes);
    writer.writeString(valueType);
    writer.writeString(value);
    ASSERT_TRUE(writer.valid());

    StringContainer message2 = ToRawString(
                                   ToRawU8(CMD_PUT) +
                                   ToRawU32(0) +
                                   ToRawString(key) +
                                   ToRawString(attributes) +
                                   ToRawString(valueType) +
                                   ToRawString(value)
                               );
    EXPECT_EQ(message, message2);
}
}
