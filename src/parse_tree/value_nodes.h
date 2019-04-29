#ifndef __VALUE_NODES_H_
#define __VALUE_NODES_H_

#include "basic_nodes.h"


/**
 * The node class holding a data type in the parse tree.
 */
struct TypeNode : public StatementNode {
    DataType type;

    TypeNode(const Location& loc, DataType type) : StatementNode(loc) {
        this->type = type;
    }

    virtual bool analyze(ScopeContext* context) {
        return true;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << Utils::dtypeToStr(type);
    }
};

/**
 * The node class holding a value in the parse tree.
 */
struct ValueNode : public ExpressionNode {
    string value;

    ValueNode(const Location& loc, DataType type, const char* value) : ExpressionNode(loc) {
        this->type = type;
        this->value = value;
    }

    virtual bool analyze(ScopeContext* context) {
        return true;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << value;
    }
};

/**
 * The node class holding an identifier in the parse tree.
 */
struct IdentifierNode : public ExpressionNode {
    string name;

    IdentifierNode(const Location& loc, const char* name) : ExpressionNode(loc) {
        this->name = name;
    }

    virtual bool analyze(ScopeContext* context) {
        Symbol* ptr = context->getSymbol(name);

        if (ptr == NULL) {
            context->printError("'" + name + "' was not declared in this scope", loc);
            return false;
        } else {
            type = ptr->type;
            return true;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << name;
    }
};

#endif