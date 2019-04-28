#ifndef __VALUE_NODES_H_
#define __VALUE_NODES_H_

#include "basic_nodes.h"


/**
 * The node class holding a data type in the parse tree.
 */
struct TypeNode : public StatementNode {
    DataType type;
    Location loc;

    TypeNode(DataType type, const Location& loc) {
        this->type = type;
        this->loc = loc;
    }

    virtual ~TypeNode() {

    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << Utils::dtypeToStr(type);
    }
};

/**
 * The node class holding a value in the parse tree.
 */
struct ValueNode : public ExpressionNode {
    DataType type;
    string value;
    Location loc;

    ValueNode(DataType type, const char* value, const Location& loc) {
        this->type = type;
        this->value = value;
        this->loc = loc;
    }

    virtual ~ValueNode() {

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
    Location loc;

    IdentifierNode(const char* name, const Location& loc) {
        this->name = name;
        this->loc = loc;
    }

    virtual ~IdentifierNode() {

    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << name;
    }
};

#endif