#ifndef __VALUE_NODES_H_
#define __VALUE_NODES_H_

#include "basic_nodes.h"


/**
 * The node class holding a data type in the parse tree.
 */
struct TypeNode : public StatementNode {
    Token token;
    DataType type;

    TypeNode(const Token& token, DataType type) {
        this->token = token;
        this->type = type;
    }

    virtual ~TypeNode() {

    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << token.value;
    }
};

/**
 * The node class holding a value in the parse tree.
 */
struct ValueNode : public ExpressionNode {
    Token token;
    DataType type;

    ValueNode(const Token& token, DataType type) {
        this->token = token;
        this->type = type;
    }

    virtual ~ValueNode() {

    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << token.value;
    }
};

/**
 * The node class holding an integer value in the parse tree.
 */
struct IntNode : public ExpressionNode {
    int value;

    IntNode(int value) {
        this->value = value;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << value;
    }
};

/**
 * The node class holding a floating value in the parse tree.
 */
struct FloatNode : public ExpressionNode {
    float value;

    FloatNode(float value) {
        this->value = value;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << value;
    }
};

/**
 * The node class holding a character value in the parse tree.
 */
struct CharNode : public ExpressionNode {
    char value;

    CharNode(char value) {
        this->value = value;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << value;
    }
};

/**
 * The node class holding a boolean value in the parse tree.
 */
struct BoolNode : public ExpressionNode {
    bool value;

    BoolNode(bool value) {
        this->value = value;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << value;
    }
};

/**
 * The node class holding a variable use in the parse tree.
 */
struct VarNode : public ExpressionNode {
    string name;

    VarNode(const char* name) {
        this->name = string(name);
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << name;
    }
};

#endif