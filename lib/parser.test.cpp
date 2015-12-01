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

#include "parser.h"
#include "simplestring.h"
#include "stringparse.h"

#include <functional>
#include <iterator>
#include <unistd.h>  // NOLINT
#include <gtest/gtest.h>

#include <map>
#include <string>
#include <ostream>
#include <iostream>

#include <cmath>

using namespace bitpowder::lib;
using namespace std;

namespace tests {

struct Number {
    enum Type {INT, FLOAT, ERROR};
    Type type;
    union {
        int intValue;
        double floatValue;
    };
    Number(Type t = INT) : type(t), intValue(0) {
    }
    Number(double f) : type(::isnan(f) ? ERROR : FLOAT), floatValue(f) {
    }
    Number(int i) : type(INT), intValue(i) {
    }

    operator int() const {
        return getInt();
    }
    operator double() const {
        return getFloat();
    }
    double getFloat() const {
        return type == FLOAT ? floatValue : intValue;
    }
    int getInt() const {
        return type == FLOAT ? floatValue : intValue;
    }
    bool isFloat() const {
        return type == FLOAT;
    }
    bool isInt() const {
        return type == INT;
    }
    bool isError() const {
        return type == ERROR;
    }

    Number operator+(const Number& n) const {
        if (isError() || n.isError())
            return Number(ERROR);
        if (isFloat() || n.isFloat())
            return getFloat() + n.getFloat();
        return getInt() + n.getInt();
    }
    Number operator-(const Number& n) const {
        if (isError() || n.isError())
            return Number(ERROR);
        if (isFloat() || n.isFloat())
            return getFloat() - n.getFloat();
        return getInt() - n.getInt();
    }
    Number operator*(const Number& n) const {
        if (isError() || n.isError())
            return Number(ERROR);
        if (isFloat() || n.isFloat())
            return getFloat() * n.getFloat();
        return getInt() * n.getInt();
    }
    Number operator/(const Number& n) const {
        if (isError() || n.isError())
            return Number(ERROR);
        if ((n.isFloat() && n.getFloat() == 0) ||
                (n.isInt() && n.getInt() == 0))
            return Number(ERROR);
        if (isInt() && n.isInt() && getInt() % n.getInt() == 0)
            return getInt() / n.getInt();
        return getFloat() / n.getFloat();
    }
    Number operator%(const Number& n) const {
        if (isError() || n.isError())
            return Number(ERROR);
        if ((n.isFloat() && n.getFloat() == 0) ||
                (n.isInt() && n.getInt() == 0))
            return Number(ERROR);
        if (isFloat() || n.isFloat())
            return Number(ERROR);
        return getInt() % n.getInt();
    }
    bool operator==(const Number& n) const {
        if (isError() && n.isError())
            return true;
        if (isFloat() || n.isFloat())
            return getFloat() == n.getFloat();
        return getInt() == n.getInt();
    }
    Number operator-() const {
        if (isError())
            return Number(ERROR);
        return isFloat() ? -getFloat() : -getInt();
    }
    Number pow(const Number& n) const {
        if (isError() || n.isError())
            return Number(ERROR);
        return ::pow(getFloat(), n.getFloat());
    }
    Number sqrt() const {
        if (isError())
            return Number(ERROR);
        if (isInt()) {
            int candidate = ::sqrt(getFloat());
            if (candidate*candidate == getInt())
                return candidate;
        }
        return ::sqrt(getFloat());
    }
    Number log2() const {
        if (isError())
            return Number(ERROR);
        return ::log2(getFloat());
    }
    Number sin() const {
        if (isError())
            return Number(ERROR);
        return ::sin(getFloat());
    }
    Number cos() const {
        if (isError())
            return Number(ERROR);
        return ::cos(getFloat());
    }
    Number tan() const {
        if (isError())
            return Number(ERROR);
        return ::tan(getFloat());
    }

    void print(std::ostream& out) const {
        if (isError())
            out << "(error)";
        else if (isFloat())
            out << getFloat() << "f";
        else
            out << getInt();
    }
};

}

/*
DECLARE_TYPE_HASH(Token<tests::Number>, 1);
DECLARE_TYPE_HASH(Token<char>, 2);
DECLARE_TYPE_HASH(Token<int>, 3);
DECLARE_TYPE_HASH(Token<String>, 4);
*/

namespace tests {

class MyLexer {
    String original;
    String c;
    Token<Number> numberToken;
    Token<char> opToken;
    void whitespace() {
        static auto space = C(' ');
        StringParser(c).span(space).remainder(c);
    }
  public:
    static const Token<char> SIN_FUNC;
    static const Token<char> COS_FUNC;
    static const Token<char> TAN_FUNC;
    static const Token<char> SQRT_FUNC;
    static const Token<char> LOG_FUNC;
    static const Token<char> POW_FUNC;
    static const Token<char> INLINE_POW;
    static const Token<char> INLINE_MUL;
    static const Token<char> INLINE_DIV;
    static const Token<char> INLINE_MOD;
    static const Token<char> OPEN;
    static const Token<char> CLOSE;
    static const Token<char> COMMA;
    static const Token<char> PERCENTAGE;
    MyLexer(const String& str) : original(str), c(str) {
    }
    /*
    bool end() {
        whitespace();
        return c.empty();
    }
    */
    TokenBase* next() {
        whitespace();
        const char* currentPtr = c.pointer();
        TokenBase* retval = recognise();
        if (retval != nullptr) {
            retval->start = currentPtr - original.pointer();
            retval->length = c.pointer() - currentPtr;
        }
        return retval;
    }
    TokenBase* recognise() {
        if (c.length() == 0)
            return nullptr;
        if (StringParser(c).accept("pi").remainder(c)) {
            numberToken.value = Number(M_PI);
            return &numberToken;
        }
        if (StringParser(c).accept("e").remainder(c)) {
            numberToken.value = Number(M_E);
            return &numberToken;
        }
        if (StringParser(c).accept("sin").remainder(c)) {
            opToken.value = SIN_FUNC;
            return &opToken;
        }
        if (StringParser(c).accept("cos").remainder(c)) {
            opToken.value = COS_FUNC;
            return &opToken;
        }
        if (StringParser(c).accept("tan").remainder(c)) {
            opToken.value = TAN_FUNC;
            return &opToken;
        }
        if (StringParser(c).accept("sqrt").remainder(c)) {
            opToken.value = SQRT_FUNC;
            return &opToken;
        }
        if (StringParser(c).accept("log").remainder(c)) {
            opToken.value = LOG_FUNC;
            return &opToken;
        }
        if (StringParser(c).accept("pow").remainder(c)) {
            opToken.value = POW_FUNC;
            return &opToken;
        }
        if (StringParser(c).accept("mod").remainder(c)) {
            opToken.value = INLINE_MOD;
            return &opToken;
        }
        if (c[0] == INLINE_POW
                || c[0] == INLINE_MUL || c[0] == INLINE_DIV
                || c[0] == '+' || c[0] == '-' ||
                c[0] == OPEN || c[0] == CLOSE || c[0] == COMMA || c[0] == PERCENTAGE) {
            opToken.value = c[0];
            c = c.substring(1);
            //std::cout << "token: " << opToken.value << std::endl;
            return &opToken;
        }
        int intValue;
        if (StringParser(c).parseNumber(intValue).remainder(c)) {
            //std::cout << "token: " << intToken.value << stdcurrentendl;
            numberToken.value = intValue;
            return &numberToken;
        }
        return nullptr;
    }
    String getInputString(TokenBase* token) {
        return token ? original.substring(token->start, token->length) : "(none)";
    }
};

const Token<char> MyLexer::SIN_FUNC = 'a';
const Token<char> MyLexer::COS_FUNC = 'b';
const Token<char> MyLexer::TAN_FUNC = 'c';
const Token<char> MyLexer::SQRT_FUNC = 's';
const Token<char> MyLexer::LOG_FUNC = 'l';
const Token<char> MyLexer::POW_FUNC = 'p';
const Token<char> MyLexer::INLINE_POW = '^';
const Token<char> MyLexer::INLINE_MUL = '*';
const Token<char> MyLexer::INLINE_DIV = '/';
const Token<char> MyLexer::INLINE_MOD = 'm';
const Token<char> MyLexer::OPEN = '(';
const Token<char> MyLexer::CLOSE = ')';
const Token<char> MyLexer::COMMA = ',';
const Token<char> MyLexer::PERCENTAGE = '%';

template <class Lexer>
struct NumberExpression {
    typedef void* UserData;
    typedef Parser<Lexer, 1, Number, UserData>& PS;
    static const int ERROR_IN_CONSTANT = 10;
    static const int ERROR_IN_NEGATIVE_CONSTANT = 11;
    static const int ERROR_IN_PERCENTAGE = 12;
    static const int ERROR_IN_HAAKJES_BEGIN = 15;
    static const int ERROR_IN_HAAKJES_END = 16;
    static const int ERROR_IN_POW = 20;
    static const int ERROR_IN_POW_FUNC = 21;
    static const int ERROR_IN_LOG_FUNC = 22;
    static const int ERROR_IN_MUL = 23;
    static const int ERROR_IN_DIV = 24;
    static const int ERROR_IN_PLUS = 25;
    static const int ERROR_IN_MIN = 26;
    static const int ERROR_IN_SQRT_FUNC = 27;
    static const int ERROR_IN_SIN_FUNC = 28;
    static const int ERROR_IN_COS_FUNC = 29;
    static const int ERROR_IN_TAN_FUNC = 30;
    static const int ERROR_IN_MOD = 24;

    static int constant(Number& retval, const Token<Number>& t, UserData userData) {
        //std::cout << "number: ";
        //t.value.print(std::cout);
        //std::cout << std::endl;
        retval = Number(t.value);
        return 0;
    }

    static PS negativeConstant(PS cont, UserData userData) {
        return cont()
               .accept('-').error(ERROR_IN_NEGATIVE_CONSTANT)
               .perform(primary)
        .modify([](Number &value, UserData) {
            value = -value;
            return 0;
        });
    }

    static PS haakjes(PS cont, UserData userData) {
        return cont()
               .perform(MyLexer::OPEN, expr, MyLexer::CLOSE)
               .error(ERROR_IN_HAAKJES_END);
    }

    static PS sinFuncOp(PS cont, UserData userData) {
        return cont().accept(Lexer::SIN_FUNC).error(ERROR_IN_SIN_FUNC).perform(haakjes).modify([](Number &b, UserData) {
            b = b.sin();
            return 0;
        });
    }

    static PS cosFuncOp(PS cont, UserData userData) {
        return cont().accept(Lexer::COS_FUNC).error(ERROR_IN_COS_FUNC).perform(haakjes).modify([](Number &b, UserData) {
            b = b.cos();
            return 0;
        });
    }

    static PS tanFuncOp(PS cont, UserData userData) {
        return cont().accept(Lexer::TAN_FUNC).error(ERROR_IN_TAN_FUNC).perform(haakjes).modify([](Number & b, UserData) {
            b = b.tan();
            return 0;
        });
    }

    static PS sqrtFuncOp(PS cont, UserData userData) {
        return cont().accept(Lexer::SQRT_FUNC).error(ERROR_IN_SQRT_FUNC).perform(haakjes).modify([](Number & b, UserData) {
            b = b.sqrt();
            return 0;
        });
    }

    static PS logFuncOp(PS cont, UserData userData) {
        return cont().accept(Lexer::LOG_FUNC).error(ERROR_IN_LOG_FUNC).perform(haakjes).modify([](Number & b, UserData) {
            b = b.log2();
            return 0;
        });
    }

    static PS powFuncOp(PS cont, UserData userData) {
        return cont().perform(Lexer::POW_FUNC, Lexer::OPEN, expr, Lexer::COMMA).process(expr, [](Number & a, const Number & b, UserData) {
            a = a.pow(b);
            return 0;
        }).perform(Lexer::CLOSE).error(ERROR_IN_POW_FUNC);
    }

    static PS primary(PS cont, UserData userData) {
        return cont().choose(constant, negativeConstant, haakjes, sinFuncOp, cosFuncOp, tanFuncOp, sqrtFuncOp, logFuncOp, powFuncOp);
    }

    static PS percentageOp(PS cont, UserData userData) {
        return cont().accept(Lexer::PERCENTAGE).error(ERROR_IN_PERCENTAGE).modify([](Number & n, UserData userData) {
            n = n/Number(100);
            return 0;
        });
    }

    static PS percentage(PS cont, UserData userData) {
        return cont().perform(primary).opt(percentageOp);
    }

    static PS powOp(PS cont, UserData userData) {
        return cont().accept(Lexer::INLINE_POW).error(ERROR_IN_POW).process(pow, [](Number & a, Number && b, UserData userData) {
            a = a.pow(b);
            return 0;
        });
    }

    static PS powTail(PS cont, UserData userData) {
        return cont().choose(powOp).opt(powTail);
    }

    static PS pow(PS cont, UserData userData) {
        return cont().perform(percentage).opt(powTail);
    }

    static PS mulOp(PS cont, UserData userData) {
        return cont().accept(Lexer::INLINE_MUL).error(ERROR_IN_MUL).process(pow, [](Number & a, const Number & b, UserData userData) {
            a = a*b;
            return 0;
        });
    }

    static PS divOp(PS cont, UserData userData) {
        return cont().accept('/').error(ERROR_IN_DIV).process(pow, [](Number & a, Number && b, UserData userData) {
            a = a/b;
            return 0;
        });
    }

    static PS modOp(PS cont, UserData userData) {
        return cont().accept(Lexer::INLINE_MOD).error(ERROR_IN_MOD).process(pow, [](Number & a, Number && b, UserData userData) {
            a = a % b;
            return 0;
        });
    }

    // multiplicationTail = ('*' pow | '/' pow) [multiplicationTail]
    static PS multiplicationTail(PS cont, UserData userData) {
        return cont().choose(mulOp, divOp, modOp).opt(multiplicationTail);
    }

    // multiplication =  pow [multiplicationTail]
    static PS multiplication(PS cont, UserData userData) {
        return cont().perform(pow).opt(multiplicationTail);
    }

    static PS plusOp(PS cont, UserData userData) {
        return cont().accept('+').process(multiplication, [](Number &a, Number &&b, UserData userData) {
            a = a + b;
            return 0;
        });
    }

    static PS minOp(PS cont, UserData userData) {
        return cont().accept('-').process(multiplication, [](Number &a, Number &&b, UserData userData) {
            a = a - b;
            return 0;
        });
    }

    // additionTail = '+' multiplication [additionTail] | '-' multiplication [additionTail]
    static PS additionTail(PS cont, UserData userData) {
        return cont().choose(plusOp, minOp).opt(additionTail);
    }

    // addition = multiplication [additionTail]
    static PS addition(PS cont, UserData userData) {
        return cont().perform(multiplication).opt(additionTail);
    }

    static PS expr(PS cont, UserData userData) {
        return cont().perform(addition);
    }
};

int ParseIntegerExpression(const String& str, Number& result) {
    std::cout << "parsing: " << str << std::endl;
    MyLexer lexer(str);
    ParserState<MyLexer, 1> p = ParserState<MyLexer, 1>(lexer);
    Parser<MyLexer, 1, Number, NumberExpression<MyLexer>::UserData> parser(&p, nullptr);
    int retval = parser.perform(NumberExpression<MyLexer>::expr).end().retreive(result);
    if (retval != 0 || result.isError()) {
        std::cout << "parsing error in " << str << " code=" << retval << " part='" << lexer.getInputString(p.getToken()) << "' result=";
        result.print(std::cout);
        std::cout << std::endl;
    }
    return retval;
}

#define EXPECT_INT_EQ(a, b) EXPECT_EQ((int)a, (int)b)

TEST(Parser, CalculatorOfPositiveValuesFlexible) {
    Number result = -1;

    EXPECT_EQ(ParseIntegerExpression("1", result), 0);
    EXPECT_EQ(Number(1), result);

    EXPECT_EQ(ParseIntegerExpression("-1", result), 0);
    EXPECT_EQ(Number(-1), result);

    EXPECT_EQ(ParseIntegerExpression("pi", result), 0);
    EXPECT_DOUBLE_EQ(M_PI, result);

    EXPECT_EQ(ParseIntegerExpression("-pi", result), 0);
    EXPECT_DOUBLE_EQ(-M_PI, result);

    EXPECT_EQ(ParseIntegerExpression("(1)", result), 0);
    EXPECT_EQ(Number(1), result);

    EXPECT_EQ(ParseIntegerExpression("-(1)", result), 0);
    EXPECT_EQ(Number(-1), result);

    EXPECT_EQ(ParseIntegerExpression("(pi)", result), 0);
    EXPECT_DOUBLE_EQ(M_PI, result);

    EXPECT_EQ(ParseIntegerExpression("-(pi)", result), 0);
    EXPECT_DOUBLE_EQ(-M_PI, result);

    EXPECT_EQ(ParseIntegerExpression("100%", result), 0);
    EXPECT_EQ(Number(1), result);

    EXPECT_EQ(ParseIntegerExpression("50*100%", result), 0);
    EXPECT_EQ(Number(50), result);

    EXPECT_EQ(ParseIntegerExpression("1* 1%", result), 0);
    EXPECT_DOUBLE_EQ(0.01, result);

    EXPECT_EQ(ParseIntegerExpression("2^100%", result), 0);
    EXPECT_EQ(Number(2), result);

    EXPECT_EQ(ParseIntegerExpression("sqrt(16)", result), 0);
    EXPECT_EQ(Number(4), result);

    EXPECT_EQ(ParseIntegerExpression("sqrt(2)", result), 0);
    EXPECT_DOUBLE_EQ(sqrt(2), result);

    EXPECT_EQ(ParseIntegerExpression("sin(pi)", result), 0);
    EXPECT_DOUBLE_EQ(sin(M_PI), result);

    EXPECT_EQ(ParseIntegerExpression("cos(pi)", result), 0);
    EXPECT_DOUBLE_EQ(cos(M_PI), result);

    EXPECT_EQ(ParseIntegerExpression("tan(pi)", result), 0);
    EXPECT_DOUBLE_EQ(tan(M_PI), result);

    EXPECT_EQ(ParseIntegerExpression("16 mod 2", result), 0);
    EXPECT_EQ(Number(0), result);

    EXPECT_EQ(ParseIntegerExpression("15 mod 7", result), 0);
    EXPECT_EQ(Number(1), result);

    // random stuff

    EXPECT_EQ(ParseIntegerExpression("1+2", result), 0);
    EXPECT_INT_EQ(result, 3);

    EXPECT_EQ(ParseIntegerExpression("1+-2", result), 0);
    EXPECT_INT_EQ(result, -1);

    EXPECT_EQ(ParseIntegerExpression("1-2", result), 0);
    EXPECT_INT_EQ(result, -1);


    EXPECT_EQ(ParseIntegerExpression(" 3 * ( 3 + 2 ) * ( 2 + 1 ) * 1 ", result), 0);
    EXPECT_INT_EQ(result, 45);

    EXPECT_EQ(ParseIntegerExpression(" 3 * ( 3 + 2 ) * ( 2 + 1 - 5 + 7 - 2) * (3-2) / 5", result), 0);
    EXPECT_INT_EQ(result, 9);

    EXPECT_EQ(ParseIntegerExpression(" 3 * ( 3 + 2 ) * ( 2 + 1 + -5 + 7 + -2) * (3+-2)", result), 0);
    EXPECT_INT_EQ(result, 45);

    EXPECT_EQ(ParseIntegerExpression("2^4", result), 0);
    EXPECT_INT_EQ(result, 16);

    EXPECT_EQ(ParseIntegerExpression("(2^2)^3", result), 0);
    EXPECT_INT_EQ(result, 64);

    EXPECT_EQ(ParseIntegerExpression("2^2^3", result), 0);
    EXPECT_INT_EQ(result, 256);

    EXPECT_EQ(ParseIntegerExpression("pow(2,4)", result), 0);
    EXPECT_INT_EQ(result, 16);

    EXPECT_EQ(ParseIntegerExpression("1/0", result), 0);
    EXPECT_EQ(result, Number(Number::ERROR));

    EXPECT_EQ(ParseIntegerExpression("log(16)", result), 0);
    EXPECT_INT_EQ(result, 4);
}

struct Operator {
    char op;

    Operator(char op = 0) : op(op) {
    }

    bool operator==(const Operator& b) const {
        return op == b.op;
    }
};


class LispLexer {
    String original;
    StringParser c;
    Token<String> stringToken;
    Token<int> numberToken;
    Token<Operator> opToken;

    constexpr static auto space = C(' ') + C('\n') + C('\t') + C('\r');

    void whitespace() {
        c.span(space);
    }
  public:
    static const Operator FUNC_OPEN;
    static const Operator FUNC_CLOSE;
    static const Operator NIL_OBJECT;

    LispLexer(const String& str) : original(str), c(str, 0) {
    }
    TokenBase* next() {
        whitespace();
        const char* currentPtr = c.remaining().pointer();
        TokenBase* retval = recognise();
        if (retval != nullptr) {
            retval->start = currentPtr - original.pointer();
            retval->length = c.remaining().pointer() - currentPtr;
        }
        return retval;
    }
    TokenBase* recognise() {
        if (StringParser(c).accept("nil"_S).remainder(c)) {
            opToken.value = NIL_OBJECT;
            return &opToken;
        }
        if (StringParser(c).accept(FUNC_OPEN.op).remainder(c)) {
            opToken.value = FUNC_OPEN;
            return &opToken;
        }
        if (StringParser(c).accept(FUNC_CLOSE.op).remainder(c)) {
            opToken.value = FUNC_CLOSE;
            return &opToken;
        }
        if (StringParser(c).accept('"').splitOn(stringToken.value, C('"')).remainder(c)) {
            return &stringToken;
        }
        if (StringParser(c).parseNumber(numberToken.value).remainder(c))
            return &numberToken;
        if (StringParser(c).splitOn(stringToken.value, space).remainder(c) && stringToken.value.size() > 0)
            return &stringToken;
        return nullptr;
    }
    String getInputString(TokenBase* token) {
        return token ? original.substring(token->start, token->length) : "(none)";
    }
};

constexpr decltype(LispLexer::space) LispLexer::space;

const Operator LispLexer::NIL_OBJECT = 'n';
const Operator LispLexer::FUNC_OPEN = '(';
const Operator LispLexer::FUNC_CLOSE = ')';

template <class Lexer = LispLexer>
struct LispParserState {
    typedef int Data;
    typedef MemoryPool& UserData;
    typedef Parser<Lexer, 1, Data, UserData>& PS;
    static const int ERROR_IN_CONSTANT = 10;

    // nil -> int = Operator(LispLexer::NIL_OBJECT) {{ nil = 0; return 0; }};
    static PS lispNil(PS cont, UserData userData) {
        return cont().accept(LispLexer::NIL_OBJECT).store(0);
    }

    // number -> int = int;
    static int lispNumber(int& retval, const Token<int>& t, UserData userData) {
        retval = t.value;
        return 0;
    }

    // string -> int = String {{ string = 42; return 0; }};
    static int lispString(int& retval, const Token<String>& t, UserData userData) {
        retval = 42;
        return 0;
    }

    // primary -> int = nil | number | string;
    static PS primary(PS cont, UserData userData) {
        return cont().choose(lispNil, lispNumber, lispString);
    }

    // arguments -> std::vector<int> = a:expr { arguments.push_back(a); return 0; } arguments?;
    static Parser<Lexer, 1, std::vector<int>, UserData>& arguments(Parser<Lexer, 1, std::vector<int>, UserData>& cont, UserData userData) {
        return cont().process(expr, [](std::vector<int>& data, int a, UserData userData) {
            std::cout << "parsed " << a << std::endl;
            data.push_back(a);
            return 0;
        }).opt(arguments);
    }

    /*
     * func -> int = char('(') String:id expr:a [std::vector<int> = (a:expr {{ arguments.push_back(a); return 0; }})*]:arguments char(')') {{
     * func -> int = char('(') String:id expr:a arguments:t char(')') {{
     *          if ("-"_S == id) for (int i : t) a -= i;
     *          else if ("+"_S == id) for (int i : t) a += i;
     *          else return 1;
     *          func = a;
     *          return 0;
     *      }};
     */
    static PS func(PS cont, UserData userData) {
        std::vector<int> args;
        String id;
        return cont().accept(LispLexer::FUNC_OPEN).fetch(id).perform(expr).fetch(arguments, args).accept(LispLexer::FUNC_CLOSE).modify([&id, &args](int& a, UserData) {
            std::cout << "function has " << args.size() << " arguments" << std::endl;
            if ("-"_S == id) {
                for (int i : args)
                    a -= i;
            } else if ("+"_S == id) {
                for (int i : args)
                    a += i;
            } else {
                return -4;
            }
            std::cout << "result is " << a << std::endl;
            return 0;
        });
    }

    // expr -> int = primary | func;
    static PS expr(PS cont, UserData userData) {
        return cont().choose(primary, func);
    }
    // topLevel -> int = expr $;
    static PS topLevelExpr(PS cont, UserData userData) {
        return cont().choose(expr).end();
    }
    // UserData := MemoryPool&;
};

int ParseLispExpression(const String& str, int& result, MemoryPool& mp) {
    //std::cout << "parsing: " << str << std::endl;
    LispLexer lexer(str);
    ParserState<LispLexer, 1> p = ParserState<LispLexer, 1>(lexer);
    int retval = Parser<LispLexer, 1, int, MemoryPool&>(&p, mp).perform(LispParserState<LispLexer>::topLevelExpr).end().retreive(result);
    if (retval != 0) {
        std::cout << "parsing error in " << str << " code=" << retval << " part='" << lexer.getInputString(p.getToken()) << "' result=";
        std::cout << result;
        //result.print(std::cout);
        std::cout << std::endl;
    }
    return retval;
}


TEST(Parser, LispParserState) {
    MemoryPool mp;
    int result = 0;

    EXPECT_EQ(0, ParseLispExpression("nil", result, mp));
    EXPECT_EQ(0, result);

    EXPECT_EQ(0, ParseLispExpression("10", result, mp));
    EXPECT_EQ(10, result);

    EXPECT_EQ(0, ParseLispExpression("\"lala\"", result, mp));
    EXPECT_EQ(42, result);

    EXPECT_EQ(0, ParseLispExpression("(+ 1 2 3 4 5)", result, mp));
    EXPECT_EQ(15, result);

    EXPECT_EQ(0, ParseLispExpression("(+ 1 (- 3 2) 4 5)", result, mp));
    EXPECT_EQ(11, result);

    EXPECT_EQ(0, ParseLispExpression("(+ (- 3 2) (- 5 4) (- 6 5))", result, mp));
    EXPECT_EQ(3, result);

    EXPECT_EQ(-4, ParseLispExpression("(range-in (mod (- in 4) 8) 1 2)", result, mp));
    EXPECT_EQ(3, result);
}

}

namespace std {
inline std::ostream& operator<< (std::ostream& out, const tests::Number& n) {
    if (n.isError())
        out << "(error)";
    else if (n.isFloat())
        out << n.getFloat() << "f";
    else
        out << n.getInt();
    return out;
}

inline std::ostream& operator<< (std::ostream& out, const tests::Operator& op) {
    out << "(operator " << op.op << ")";
    return out;
}

}
