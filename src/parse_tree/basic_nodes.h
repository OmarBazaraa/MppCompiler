#ifndef __BASIC_NODES_H_
#define __BASIC_NODES_H_

#include <iostream>
#include <string>
#include <vector>

#include "../utils/consts.h"
#include "../utils/utils.h"


//
// Prototypes
//
struct ScopeContext;
struct GenerationContext;

struct Node;
struct StatementNode;
struct DeclarationNode;
struct VarDeclarationNode;
struct FunctionNode;
struct ExpressionNode;
struct IdentifierNode;
struct TypeNode;

typedef vector<Node*> NodeList;
typedef vector<StatementNode*> StmtList;
typedef vector<VarDeclarationNode*> VarList;
typedef vector<ExpressionNode*> ExprList;


/**
 * The base class of all nodes in the parse tree.
 */
struct Node {
    Location loc;

    Node() {}

    Node(const Location& loc) {
        this->loc = loc;
    }

    virtual ~Node() {}

    virtual bool analyze(ScopeContext* context) {
        return true;
    }

    virtual string generateQuad(GenerationContext* context) {
        return "";
    }

    virtual string toString() {
        return "";
    }
};

/**
 * The base class of all statement nodes in the parse tree.
 */
struct StatementNode : public Node {

    StatementNode() {}

    StatementNode(const Location& loc) : Node(loc) {}

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + ";" ;
    }
};

/**
 * The base class of all declaration statement nodes in the parse tree.
 */
struct DeclarationNode : public StatementNode {
    TypeNode* type;
    IdentifierNode* ident;

    //
    // NOTE: the following variables will be computed after calling analyze function
    //
    string alias;                       // Alias name to avoid same identifier in different scopes
    int used = 0;                       // The number of times this declaration node has been read
    bool initialized = false;           // Whether this declaration node has been initialized or not

    DeclarationNode(const Location& loc) : StatementNode(loc) {}

    virtual string declaredHeader() = 0;

    virtual string declaredType() = 0;
};

/**
 * The base class of all expression nodes in the parse tree.
 */
struct ExpressionNode : public StatementNode {
    //
    // NOTE: the following variables will be computed after calling analyze function
    //
    DataType type = DTYPE_ERROR;        // Data type of the expression
    DeclarationNode* reference = NULL;  // Reference variable of the expression is exist
    bool constant = false;              // Whether the expression is of constant value or not
    bool used = false;                  // Whether the value of the expression is to be used or not

    ExpressionNode() {}

    ExpressionNode(const Location& loc) : StatementNode(loc) {}

    virtual int getConstIntValue() {
        return -1;
    }
    
    virtual bool analyze(ScopeContext* context) {
        return analyze(context, false);
    }

    virtual bool analyze(ScopeContext* context, bool valueUsed) {
        used = valueUsed;
        return true;
    }

    virtual string exprTypeStr() {
        return reference ? reference->declaredType() : Utils::dtypeToStr(type);
    }
};

/**
 * The node class holding a data type in the parse tree.
 */
struct TypeNode : public Node {
    DataType type;

    TypeNode(const Location& loc, DataType type) : Node(loc) {
        this->type = type;
    }

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + Utils::dtypeToStr(type);
    }
};

/**
 * The node class representing a syntax error statement.
 */
struct ErrorNode : public StatementNode {
    string what;

    ErrorNode(const Location& loc, const string& what) : StatementNode(loc) {
        this->what = what;
        this->loc.pos -= this->loc.len - 1;
    }

    virtual bool analyze(ScopeContext* context);

    virtual string toString(int ind) {
        return string(ind, ' ') + ">> ERROR" ;
    }
};

#endif