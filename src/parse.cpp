//#define PARSER_DEBUG

#include "parse.h"
#include "parser.h"
#include "stringparse.h"

#include <sys/stat.h>
#include <fcntl.h>

/**
 * TODO:
 * + assignments
 * - comparisons
 * + function calls
 * + function definitions
 * - parsing inputs
 *
 * Future:
 * - negate unary operator
 * - making else optional
 * - ...
 */

using namespace bitpowder::lib;

const static int ECA_LOOKAHEAD = 2;

class ECALexer {
    MemoryPool& mp;
    String original;
    String c;
    int lookaheadIndex = 0;
    Token<String> stringToken[ECA_LOOKAHEAD];
    Token<char> opToken[ECA_LOOKAHEAD];
    Token<ECAType> constantToken[ECA_LOOKAHEAD];
    Token<Exception> exceptionToken = Exception("could not Tokenize remaining value");

    void whitespace() {
        static auto space = C(' ') + C('\n') + C('\r') + C('\t');
        StringParser(c).span(space).remainder(c);
    }

    struct CStringSpanner {
        mutable bool previousCharWasEscapeChar = false;
        mutable bool didSeeEscapeChar = false;
        bool operator()(char in) const {
            bool retval = previousCharWasEscapeChar || in != '"';
            previousCharWasEscapeChar = !previousCharWasEscapeChar && in == '\\';
            didSeeEscapeChar = didSeeEscapeChar || previousCharWasEscapeChar;
            return retval; // true = accept this char
        }
    };

public:
    static const Token<char> SKIP;

    static const Token<char> DOT;

    static const Token<char> MUL;
    static const Token<char> DIV;
    static const Token<char> PLUS;
    static const Token<char> MIN;
    static const Token<char> ASSIGNMENT;
    static const Token<char> COMMA;
    static const Token<char> SEMICOLON;
    static const Token<char> OPEN;
    static const Token<char> CLOSE;

    static const Token<char> AND;
    static const Token<char> OR;

    static const Token<char> LESS;
    static const Token<char> LESS_EQ;
    static const Token<char> EQUAL;
    static const Token<char> NOT_EQUAL;
    static const Token<char> GREATER_EQ;
    static const Token<char> GREATER;

    static const Token<char> IF;
    static const Token<char> THEN;
    static const Token<char> ELSE;

    static const Token<char> BEGIN;
    static const Token<char> END;

    static const Token<char> REPEAT;
    static const Token<char> WHILE;
    static const Token<char> FUNCTION;

    ECALexer(const String &str, MemoryPool& mp) : original(str), c(str), mp(mp) { }

    TokenBase *next() {
        whitespace();
        const char *currentPtr = c.pointer();
        TokenBase *retval = recognise();
        if (retval) {
            //std::cout << "token: " << retval << " type_hash=" << retval->type << std::endl;
            retval->start = currentPtr - original.pointer();
            retval->length = c.pointer() - currentPtr;
        }
        return retval;
    }
    TokenBase *recognise() {
        if (c.length() == 0) return nullptr;

        lookaheadIndex = (lookaheadIndex+1) % ECA_LOOKAHEAD;

        if (StringParser(c).accept("skip").remainder(c)) {
            opToken[lookaheadIndex] = SKIP;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept("if").remainder(c)) {
            opToken[lookaheadIndex] = IF;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept("then").remainder(c)) {
            opToken[lookaheadIndex] = THEN;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept("else").remainder(c)) {
            opToken[lookaheadIndex] = ELSE;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept("function").remainder(c)) {
            opToken[lookaheadIndex] = FUNCTION;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept("begin").remainder(c)) {
            opToken[lookaheadIndex] = BEGIN;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept("end").remainder(c)) {
            opToken[lookaheadIndex] = END;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept("repeat").remainder(c)) {
            opToken[lookaheadIndex] = REPEAT;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept("while").remainder(c)) {
            opToken[lookaheadIndex] = WHILE;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept("and", "&&").remainder(c)) {
            opToken[lookaheadIndex] = AND;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept("or", "||").remainder(c)) {
            opToken[lookaheadIndex] = AND;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept("<=").remainder(c)) {
            opToken[lookaheadIndex] = LESS_EQ;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept(">=").remainder(c)) {
            opToken[lookaheadIndex] = GREATER_EQ;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept("!=").remainder(c)) {
            opToken[lookaheadIndex] = NOT_EQUAL;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept("==").remainder(c)) {
            opToken[lookaheadIndex] = EQUAL;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).accept(":=").remainder(c)) {
            opToken[lookaheadIndex] = ASSIGNMENT;
            return &opToken[lookaheadIndex];
        }
        if (c[0] == MUL || c[0] == DIV || c[0] == PLUS || c[0] == MIN ||
                c[0] == COMMA || c[0] == OPEN || c[0] == CLOSE || c[0] == SEMICOLON || c[0] == DOT ||
                c[0] == LESS || c[0] == GREATER) {
            opToken[lookaheadIndex].value = c[0];
            c = c.substring(1);
            //std::cout << "Token[lookaheadIndex] op: " << opToken[lookaheadIndex].value << " type=" << opToken[lookaheadIndex].type << std::endl;
            return &opToken[lookaheadIndex];
        }
        if (StringParser(c).parseNumber(constantToken[lookaheadIndex].value).remainder(c)) {
            return &constantToken[lookaheadIndex];
        }
        static auto variableName = Alpha() + C('_') + Num();
        if (StringParser(c).span(stringToken[lookaheadIndex].value, variableName).remainder(c) &&
                stringToken[lookaheadIndex].value.size() > 0) {
            return &stringToken[lookaheadIndex];
        }
        CStringSpanner stringSpanner;
        if (StringParser(c).accept('"').span(stringToken[lookaheadIndex].value, stringSpanner).accept('"').remainder(c)) {
            // should do unescaping here (as it is known if it is needed)
            if (stringSpanner.didSeeEscapeChar)
                stringToken[lookaheadIndex].value = stringToken[lookaheadIndex].value.unescape(String::Escape::LikeC, mp);
            return &stringToken[lookaheadIndex];
        }
        return &exceptionToken;
    }
    String getInputString(TokenBase *token) {
        return token ? original.substring(token->start, token->length) : "(none)";
    }
    String remaining() const { return c; }
};

const Token<char> ECALexer::SKIP = 's';
const Token<char> ECALexer::DOT = '.';
const Token<char> ECALexer::MUL = '*';
const Token<char> ECALexer::DIV = '/';
const Token<char> ECALexer::PLUS = '+';
const Token<char> ECALexer::MIN = '-';
const Token<char> ECALexer::ASSIGNMENT = '=';
const Token<char> ECALexer::COMMA = ',';
const Token<char> ECALexer::SEMICOLON = ';';
const Token<char> ECALexer::OPEN = '(';
const Token<char> ECALexer::CLOSE = ')';
const Token<char> ECALexer::IF = 'i';
const Token<char> ECALexer::THEN = 't';
const Token<char> ECALexer::ELSE = 'e';
const Token<char> ECALexer::BEGIN = 'b';
const Token<char> ECALexer::END = 'd';
const Token<char> ECALexer::REPEAT = 'r';
const Token<char> ECALexer::WHILE = 'w';
const Token<char> ECALexer::FUNCTION = 'f';
const Token<char> ECALexer::LESS = '<';
const Token<char> ECALexer::GREATER = '>';
const Token<char> ECALexer::LESS_EQ = 'l';
const Token<char> ECALexer::GREATER_EQ = 'g';
const Token<char> ECALexer::EQUAL = 'q';
const Token<char> ECALexer::NOT_EQUAL = 'n';
const Token<char> ECALexer::AND = '&';
const Token<char> ECALexer::OR = '|';

template <class Lexer>
struct ECALanguage {
    typedef ECAStatement::Ref Stmt;
    typedef ECAProgram::Ref FuncDefs;

    typedef MemoryPool& UserData;
    typedef Parser<Lexer, ECA_LOOKAHEAD, Stmt, UserData>& PStmt;
    typedef Parser<Lexer, ECA_LOOKAHEAD, FuncDefs, UserData>& PFunDefs;

    /// EXPRESSIONS
    static int string(String& retval, const Token<String>& t, UserData userData) {
        retval = t.value;
        return 0;
    }

    static int variable(Stmt &value, const Token<String> &token, UserData userData) {
        value = new ECAVariable(token.value);
        return 0;
    }

    static int constant(Stmt &value, const Token<ECAType> &token, UserData userData) {
        value = new ECAConstant(token.value);
        return 0;
    }

    static PStmt parenthesis(PStmt cont, UserData userData) {
        return cont().perform(ECALexer::OPEN, stmt, ECALexer::CLOSE);
    }

    static PStmt functionCall(PStmt cont, UserData userData) {
        String functionName;
        return cont().fetch(string, functionName)
            .accept(ECALexer::OPEN)
            .perform(stmt)
            .modify([](Stmt &a, const String& functionName, UserData userData) {
                if (!a->isExpression())
                    return -1;
                a = new ECAFunctionCall(functionName, a);
                return 0;
            }, functionName)
            .accept(ECALexer::CLOSE);
    }

    static PStmt componentFunctionCall(PStmt cont, UserData userData) {
        String componentName;
        String functionName;
        return cont()
            .fetch(string, componentName)
            .accept(ECALexer::DOT)
            .fetch(string, functionName)
            .accept(ECALexer::OPEN)
            .perform(stmt)
            .modify([](Stmt &a, const String& componentName, const String& functionName, UserData userData) {
                if (!a->isExpression())
                    return -1;
                a = new ECAComponentFunctionCall(componentName, functionName, a);
                return 0;
            }, componentName, functionName)
            .accept(ECALexer::CLOSE);
    }

    static PStmt primary(PStmt cont, UserData userData) {
        return cont().choose(functionCall, componentFunctionCall, variable, constant, parenthesis);
    }

    static PStmt mulOp(PStmt cont, UserData userData) {
        return cont().accept(ECALexer::MUL).process(primary, [](Stmt &a, Stmt &&b, UserData userData) {
            if (!a->isExpression() || !b->isExpression())
                return -1;
            a = new ECABinary({a, "*", b});
            return 0;
        });
    }

    static PStmt divOp(PStmt cont, UserData userData) {
        return cont().accept(ECALexer::DIV).process(primary, [](Stmt &a, Stmt &&b, UserData userData) {
            if (!a->isExpression() || !b->isExpression())
                return -1;
            a = new ECABinary({a, "/", b});
            return 0;
        });
    }

    static PStmt multiplication(PStmt cont, UserData userData) {
        return cont().perform(primary).repeatChoose(mulOp, divOp);
    }

    static PStmt plusOp(PStmt cont, UserData userData) {
        return cont().accept(ECALexer::PLUS).process(multiplication, [](Stmt &a, Stmt &&b, UserData userData) {
            if (!a->isExpression() || !b->isExpression())
                return -1;
            a = new ECABinary({a, "+", b});
            return 0;
        });
    }

    static PStmt minOp(PStmt cont, UserData userData) {
        return cont().accept(ECALexer::MIN).process(multiplication, [](Stmt &a, Stmt &&b, UserData userData) {
            if (!a->isExpression() || !b->isExpression())
                return -1;
            a = new ECABinary({a, "-", b});
            return 0;
        });
    }

    static PStmt additionTail(PStmt cont, UserData userData) {
        return cont().choose(plusOp, minOp);
    }

    static PStmt addition(PStmt cont, UserData userData) {
        return cont().perform(multiplication).repeat(additionTail);
    }

    static PStmt assignment(PStmt cont, UserData userData) {
        String variableName;
        return cont().fetch(string, variableName).accept(ECALexer::ASSIGNMENT).perform(expr).modify([](Stmt& a, String& variableName, UserData) {
            a = new ECAAssignment(variableName, a);
            return 0;
        }, variableName);
    }

    static PStmt exprBase(PStmt cont, UserData userData) {
        return cont.choose(assignment, addition);
    }

    static PStmt equalOp(PStmt cont, UserData userData) {
        return cont().accept(ECALexer::EQUAL)
                .process(exprBase, [](Stmt &a, Stmt&& b, UserData userData) {
            a = new ECABinary(a, "==", b);
            return 0;
        });
    }

    static PStmt notEqualOp(PStmt cont, UserData userData) {
        return cont().accept(ECALexer::NOT_EQUAL)
                .process(exprBase, [](Stmt &a, Stmt&& b, UserData userData) {
            a = new ECABinary(a, "!=", b);
            return 0;
        });
    }

    static PStmt lessOp(PStmt cont, UserData userData) {
        return cont().accept(ECALexer::LESS)
                .process(exprBase, [](Stmt &a, Stmt&& b, UserData userData) {
            a = new ECABinary(a, "<", b);
            return 0;
        });
    }

    static PStmt greaterOp(PStmt cont, UserData userData) {
        return cont().accept(ECALexer::GREATER)
                .process(exprBase, [](Stmt &a, Stmt&& b, UserData userData) {
            a = new ECABinary(a, ">", b);
            return 0;
        });
    }

    static PStmt lessEqOp(PStmt cont, UserData userData) {
        return cont().accept(ECALexer::LESS_EQ)
                .process(exprBase, [](Stmt &a, Stmt&& b, UserData userData) {
            a = new ECABinary(a, "<=", b);
            return 0;
        });
    }

    static PStmt greaterEqOp(PStmt cont, UserData userData) {
        return cont().accept(ECALexer::GREATER_EQ)
                .process(exprBase, [](Stmt &a, Stmt&& b, UserData userData) {
            a = new ECABinary(a, ">=", b);
            return 0;
        });
    }

    static PStmt comparisonExpr(PStmt cont, UserData userData) {
        return cont().perform(exprBase).optChoose(lessOp, lessEqOp, equalOp, notEqualOp, greaterEqOp, greaterOp);
    }

    static PStmt andOp(PStmt cont, UserData userData) {
        return cont().accept(ECALexer::AND)
                .process(comparisonExpr, [](Stmt &a, Stmt&& b, UserData userData) {
            a = new ECABinary(a, "&&", b);
            return 0;
        });
    }

    static PStmt orOp(PStmt cont, UserData userData) {
        return cont().accept(ECALexer::OR)
                .process(comparisonExpr, [](Stmt &a, Stmt&& b, UserData userData) {
            a = new ECABinary(a, "||", b);
            return 0;
        });
    }

    static PStmt expr(PStmt cont, UserData userData) {
        return cont().perform(comparisonExpr).repeatChoose(andOp, orOp);
    }


    /// STATEMENTS

    static PStmt stmtIfThenElse(PStmt cont, UserData userData) {
        Stmt c;
        Stmt t;
        Stmt e;
        return cont().accept(ECALexer::IF)
                .fetch(stmt, c)
                .modify([](Stmt& a, Stmt& b, UserData){
                    return !b->isExpression();
                }, c)
            .accept(ECALexer::THEN)
            .fetch(stmt, t)
            .accept(ECALexer::ELSE)
            .fetch(stmt, e)
            .accept(ECALexer::END)
            .modify([](Stmt& a, Stmt& c, Stmt& t, Stmt& e, UserData){
                a = new ECAConditional(c, t, e);
                return 0;
            }, c, t, e);
    }

    static PStmt stmtSkip(PStmt cont, UserData userData) {
        return cont().modify([](Stmt& a, UserData) {
            //std::cout << "stmtSkip" << std::endl;
        return 0;
        }).error(1).accept(ECALexer::SKIP).error(2).store(new ECASkip());
    }

    static PStmt stmtRepeat(PStmt cont, UserData userData) {
        Stmt i;
        return cont().accept(ECALexer::REPEAT).fetch(stmt, i)
                .accept(ECALexer::BEGIN).perform(stmt)
                .accept(ECALexer::END).modify([](Stmt& a, Stmt&& i, UserData) {
            if (!i->isExpression())
                return 1;
            a = new ECARepeat(i, a);
            return 0;
        }, std::move(i));
    }

    static PStmt stmtWhile(PStmt cont, UserData userData) {
        Stmt c;
        return cont().accept(ECALexer::WHILE).fetch(stmt, c)
                .accept(ECALexer::BEGIN).perform(stmt)
                .accept(ECALexer::END).modify([](Stmt& a, Stmt&& c, UserData) {
            if (!c->isExpression())
                return 1;
            a = new ECAWhile(c, a);
            return 0;
        }, std::move(c));
    }

    static PStmt stmtOptions(PStmt cont, UserData userData) {
        return cont().modify([](Stmt& a, UserData) {
            //std::cout << "stmtOptions" << std::endl;
        return 0;
        }).choose(expr, stmtIfThenElse, stmtRepeat, stmtWhile, stmtSkip);
    }

    static PStmt stmtTail(PStmt cont, UserData userData) {
        return cont().accept(ECALexer::SEMICOLON, ECALexer::COMMA).process(stmtOptions, [](Stmt& a, Stmt&& b, UserData) {
            a = new ECAConcat(a, b);
            return 0;
        });
    }

    static PStmt stmt(PStmt cont, UserData userData) {
        return cont().perform(stmtOptions).repeat(stmtTail);
    }

    /// TOP LEVEL FUNCTION DEFINITIONS

    static PFunDefs defTail(PFunDefs cont, UserData userData) {
        return cont().process(stmt, [](FuncDefs &a, Stmt&& b, UserData) {
            a->main = b;
            return 0;
        });
    }

    static PFunDefs definition(PFunDefs cont, UserData userData) {
        String functionName;
        String argumentName;
        return cont()
            .accept(ECALexer::FUNCTION)
            .fetch(string, functionName)
            .accept(ECALexer::OPEN)
            .fetch(string, argumentName)
            .accept(ECALexer::CLOSE)
            .process(stmt, [](FuncDefs &a, const String& functionName, const String& argumentName, Stmt&& body, UserData userData) {
                if (!body->isExpression())
                    return -1;
                a->functions[functionName] = new ECAFunctionDefinition(functionName, argumentName, body);
                return 0;
            }, functionName, argumentName)
            .accept(ECALexer::END);
    }

    static PFunDefs def(PFunDefs cont, UserData userData) {
        return cont().store(new ECAProgram()).repeat(definition).perform(defTail).modify([](FuncDefs& a, UserData) {
        /*
            for (auto& fd : a->functions) {
                fd.second->print(std::cout);
                std::cout << std::endl;
            }

            std::cout << "main statements:" << std::endl;
            a->main->print(std::cout);
            std::cout << std::endl;
            */
            return 0;
        });
    }
};

ECAParseResult ParseECAMemory(const String &str, MemoryPool& mp) {
    ECALexer lexer(str, mp);
    auto p = ParserState<ECALexer, ECA_LOOKAHEAD>(lexer);
    ECALanguage<ECALexer>::FuncDefs result;
    int retval = Parser<ECALexer, ECA_LOOKAHEAD, ECALanguage<ECALexer>::FuncDefs,MemoryPool &>(&p, mp).perform(ECALanguage<ECALexer>::def).end().retreive(result);
    if (retval != 0) {
        TokenBase *token = p.getToken();
        std::cout << "error: " << retval << std::endl;
        std::cout << "token: " << token << std::endl;
        std::cout << "token representing: " << lexer.getInputString(token) << std::endl;
        std::cout << "remaining: " << lexer.remaining() << std::endl;
        return {token ? token->start : 0, lexer.getInputString(token)};
    }
    // std::cout << "result of parse is " << result << std::endl;
    return {std::move(result)};
}

ECAParseResult ParseECAFile(const StringT& filename, MemoryPool& mp) {
    struct stat st;
    if (stat(filename.c_str(), &st)) {
	std::cerr << "error: " << strerror(errno) << std::endl;
	return {errno, "File not found (stat() error)"};
    }
#ifdef __MINGW32__
    int fd = open(filename.c_str(), O_RDONLY | O_BINARY);
#else
    int fd = open(filename.c_str(), O_RDONLY);
#endif
    if (!fd) {
	std::cerr << "error: " << strerror(errno) << std::endl;
	return {errno, "Could not open file (open() error)"};
    }
    char *buffer = (char *)mp.alloc(st.st_size);
    int size = read(fd, buffer, st.st_size);
    if (size != st.st_size) {
	std::cerr << "wrong number of bytes read: " << size << " instead of "
	    << st.st_size << std::endl;
	abort();
    }
    close(fd);

    String current(buffer, size);

    auto result = ParseECAMemory(current, mp);
    if (!result) {
	std::cerr << "error: " << result.error() << " at " << result.position() << std::endl;
    }
    return result;
}

#include "RuleFuncDef.h"
Rule* ECAProgram::getTypeRule() const
{
    Rule* retval = main->getTypeRule();
    for (const auto& fdentry : functions) {
        const ECAFunctionDefinition::Ref& fd = fdentry.second;
        retval = new RuleFuncDef(fd->functionName.stl(), fd->argumentName.stl(), fd->body->getTypeRule(), retval);
    }
    return retval;
}

#include "RuleSkip.h"
Rule* ECASkip::getTypeRule() const {
    return new RuleSkip();
}

#include "RuleConst.h"
Rule* ECAConstant::getTypeRule() const
{
    return new RuleConst(std::to_string(value));
}

#include "RuleVar.h"
Rule* ECAVariable::getTypeRule() const
{
    return new RuleVar(name.stl());
}

#include "RuleCallF.h"
Rule* ECAFunctionCall::getTypeRule() const
{
    return new RuleCallF(functionName.stl(), argument->getTypeRule());
}

#include "RuleCallCmpF.h"
Rule* ECAComponentFunctionCall::getTypeRule() const
{
    return new RuleCallCmpF(componentName.stl(), functionName.stl(), argument->getTypeRule());
}

#include "RuleBinOp.h"
Rule* ECABinary::getTypeRule() const
{
    return new RuleBinOp(op.stl(), lhs->getTypeRule(), rhs->getTypeRule());
}

#include "RuleAssign.h"
Rule* ECAAssignment::getTypeRule() const
{
    return new RuleAssign(name.stl(), rhs->getTypeRule());
}

#include "RuleIf.h"
Rule* ECAConditional::getTypeRule() const
{
    return new RuleIf(c->getTypeRule(), t->getTypeRule(), e->getTypeRule());
}

#include "RuleRepeat.h"
Rule* ECARepeat::getTypeRule() const
{
    return new RuleRepeat(b->getTypeRule(), b->getTypeRule());
}

#include "RuleWhile.h"
Rule* ECAWhile::getTypeRule() const
{
    return new RuleRepeat(c->getTypeRule(), b->getTypeRule());
}

#include "RuleExprConcat.h"
#include "RuleStmtConcat.h"
Rule* ECAConcat::getTypeRule() const
{
    if (isExpression()) {
        return new RuleExprConcat(a->getTypeRule(), b->getTypeRule());
    }
    return new RuleStmtConcat(a->getTypeRule(), b->getTypeRule());
}

#include "RuleFuncDef.h"
Rule* ECAFunctionDefinition::getTypeRule() const
{
    return new RuleFuncDef(functionName.stl(), argumentName.stl(), body->getTypeRule(), new RuleSkip());
}
