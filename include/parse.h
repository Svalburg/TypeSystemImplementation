#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <map>
#include "memorypool.h"
#include "simplestring.h"
#include "memory.h"

#include "Rule.h"

typedef int ECAType;

class ECA_AST {
public:
    int refcount = 0;
    typedef bitpowder::lib::shared_object<ECA_AST> Ref;
    virtual ~ECA_AST() {
    }
    virtual void print(std::ostream& out) const = 0;
};

class ECAStatement : public ECA_AST {
public:
    typedef bitpowder::lib::shared_object<ECAStatement, ECA_AST> Ref;
    virtual bool isExpression() const {
        return false;
    }
    virtual Rule* getTypeRule() const = 0;
};

class ECAExpression : public ECAStatement {
public:
    typedef bitpowder::lib::shared_object<ECAExpression, ECA_AST> Ref;
    virtual bool isExpression() const override {
        return true;
    }
};

class ECASkip : public ECAStatement {
public:
    ECASkip() {
    }
    virtual void print(std::ostream& out) const override {
        out << "skip";
    }
    Rule* getTypeRule() const override;
};

class ECAConstant : public ECAExpression {
    ECAType value;
public:
    ECAConstant(const ECAType& value) : value(value) {
    }
    virtual void print(std::ostream& out) const override {
        out << value;
    }
    Rule* getTypeRule() const override;
};

class ECAVariable : public ECAExpression {
    bitpowder::lib::String name;
public:
    ECAVariable(const bitpowder::lib::String& name) : name(name) {
    }
    virtual void print(std::ostream& out) const override {
        out << name;
    }
    Rule* getTypeRule() const override;
};

class ECAAssignment : public ECAExpression {
    bitpowder::lib::String name;
    ECAExpression::Ref rhs;
public:
    ECAAssignment(const bitpowder::lib::String& name, ECAExpression::Ref rhs) : name(name), rhs(rhs) {
    }
    virtual void print(std::ostream& out) const override {
        out << name << ":=";
        rhs->print(out);
    }
    Rule* getTypeRule() const override;
};

class ECABinary : public ECAExpression {
    ECAExpression::Ref lhs;
    bitpowder::lib::String op;
    ECAExpression::Ref rhs;
public:
    ECABinary(ECAExpression::Ref lhs, bitpowder::lib::String op, ECAExpression::Ref rhs) : lhs(lhs), op(op), rhs(rhs) {
    }
    virtual void print(std::ostream& out) const override {
        lhs->print(out);
        out << op;
        rhs->print(out);
    }
    Rule* getTypeRule() const override;
};

class ECAConditional : public ECAStatement {
    ECAExpression::Ref c;
    ECAStatement::Ref t;
    ECAStatement::Ref e;
public:
    ECAConditional(ECAExpression::Ref c, ECAStatement::Ref t, ECAStatement::Ref e) : c(c), t(t), e(e) {
    }
    virtual void print(std::ostream& out) const override {
        out << "if ";
        c->print(out);
        out << " then ";
        t->print(out);
        out << " else ";
        e->print(out);
        out << " end";
    }
    Rule* getTypeRule() const override;
};

class ECARepeat: public ECAStatement {
    ECAExpression::Ref i;
    ECAStatement::Ref b;
public:
    ECARepeat(ECAExpression::Ref i, ECAStatement::Ref b) : i(i), b(b) {
    }
    virtual void print(std::ostream& out) const override {
        out << "repeat ";
        i->print(out);
        out << " begin ";
        b->print(out);
        out << " end";
    }
    Rule* getTypeRule() const override;
};

class ECAWhile : public ECAStatement {
    ECAExpression::Ref c;
    ECAStatement::Ref b;
public:
    ECAWhile(ECAExpression::Ref c, ECAStatement::Ref b) : c(c), b(b) {
    }
    virtual void print(std::ostream& out) const override {
        out << "while ";
        c->print(out);
        out << " begin ";
        b->print(out);
        out << " end";
    }
    Rule* getTypeRule() const override;
};

class ECAConcat : public ECAStatement {
    ECAStatement::Ref a;
    ECAStatement::Ref b;
public:
    ECAConcat(ECAStatement::Ref a, ECAStatement::Ref b) : a(a), b(b) {
    }
    virtual void print(std::ostream& out) const override {
        a->print(out);
        out << ";";
        b->print(out);
    }
    virtual bool isExpression() const override {
        return b->isExpression();
    }
    Rule* getTypeRule() const override;
};

class ECAFunctionCall : public ECAExpression {
    bitpowder::lib::String functionName;
    ECAExpression::Ref argument;
public:
    ECAFunctionCall(const bitpowder::lib::String& functionName, ECAExpression::Ref argument) : functionName(functionName), argument(argument) {
    }
    virtual void print(std::ostream& out) const override {
        out << functionName << "(";
        argument->print(out);
        out << ")";
    }
    virtual bool isExpression() const override {
        return argument->isExpression();
    }
    Rule* getTypeRule() const override;
};

class ECAComponentFunctionCall : public ECAExpression {
    bitpowder::lib::String componentName;
    bitpowder::lib::String functionName;
    ECAExpression::Ref argument;
public:
    ECAComponentFunctionCall(const bitpowder::lib::String& componentName, const bitpowder::lib::String& functionName, ECAExpression::Ref argument) : componentName(componentName), functionName(functionName), argument(argument) {
    }
    virtual void print(std::ostream& out) const override {
        out << componentName << "." << functionName << "(";
        argument->print(out);
        out << ")";
    }
    virtual bool isExpression() const override {
        return argument->isExpression();
    }
    Rule* getTypeRule() const override;
};

class ECAFunctionDefinition : public ECAExpression {
    friend class ECAProgram;
    bitpowder::lib::String functionName;
    bitpowder::lib::String argumentName;
    ECAExpression::Ref body;
public:
    typedef bitpowder::lib::shared_object<ECAFunctionDefinition, ECA_AST> Ref;
    ECAFunctionDefinition(const bitpowder::lib::String& functionName, const bitpowder::lib::String& argumentName, ECAExpression::Ref body) : functionName(functionName), argumentName(argumentName), body(body) {
    }
    virtual void print(std::ostream& out) const override {
        out << "function " << functionName << "(" << argumentName << ") ";
        body->print(out);
        out << " end";
    }
    virtual bool isExpression() const override {
        return true;
    }
    Rule* getTypeRule() const override;
};

class ECAProgram : public ECA_AST {
public:
    std::map<bitpowder::lib::String,ECAFunctionDefinition::Ref> functions;
    ECAStatement::Ref main;

    typedef bitpowder::lib::shared_object<ECAProgram, ECA_AST> Ref;

    Rule* getTypeRule() const;

    virtual void print(std::ostream& out) const override {
        for (const auto& f : functions) {
            f.second->print(out);
            out << std::endl;
        }
        main->print(out);
    }
};

class ECAParseResult {
    bool success;
    int pos;
    bitpowder::lib::String errorMessage;
    ECAProgram::Ref retval;
public:
    ECAParseResult(int position, bitpowder::lib::String errorMessage) : success(false), pos(position), errorMessage(errorMessage), retval() {
    }
    ECAParseResult(ECAProgram::Ref &&retval) : success(true), pos(0), errorMessage(), retval(std::move(retval)) {
    }
    operator bool() {
        return success;
    }
    ECAProgram::Ref result() {
        return retval;
    }
    bitpowder::lib::String error() {
        return errorMessage;
    }
    int position() {
        return pos;
    }
};

// correct lifetime of strings in the structure is limited to the lifetime of the MemoryPool
ECAParseResult ParseECAMemory(const bitpowder::lib::String &ecaString, bitpowder::lib::MemoryPool &mp);

// correct lifetime of strings in the structure is limited to the lifetime of the MemoryPool
ECAParseResult ParseECAFile(const bitpowder::lib::StringT &filename, bitpowder::lib::MemoryPool& mp);


#endif // PARSE_H
