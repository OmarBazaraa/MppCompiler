#ifndef __BASIC_NODES_H_
#define __BASIC_NODES_H_

#include <iostream>
#include <string>
#include <vector>

#include "../symbol_table/symbol_table.h"

#include "../utils/consts.h"
#include "../utils/utils.h"


//
// Prototypes
//
struct ScopeContext;
struct GenerationContext;
struct StatementNode;
struct ExpressionNode;
struct VarDeclarationNode;
struct FunctionNode;
struct TypeNode;

typedef vector<StatementNode*> StmtList;
typedef vector<ExpressionNode*> ExprList;
typedef vector<VarDeclarationNode*> VarList;


/**
 * The base class of all statement nodes in the parse tree.
 */
struct StatementNode {
    Location loc;

    StatementNode() {}

    StatementNode(const Location& loc) {
        this->loc = loc;
    }

    ~StatementNode() {}

    virtual string toString(int ind = 0) {
        return string(ind, ' ') + ";" ;
    }

    virtual bool analyze(ScopeContext* context) {
        return true;
    }

    virtual string generateQuad(GenerationContext* generationContext) {
        return "";
    }
};

/**
 * The base class of all expression nodes in the parse tree.
 */
struct ExpressionNode : public StatementNode {
    //
    // NOTE: the following variables will be computed after calling analyze function
    //
    DataType type = DTYPE_ERROR;    // data type of the expression
    Symbol* reference = NULL;       // reference of the expression is exist
    bool isConst = false;           // whether the expression is of constant value or not
    bool used = false;              // whether the value of the expression is used or not

    ExpressionNode() {}

    ExpressionNode(const Location& loc) : StatementNode(loc) {}
    
    virtual string getType() {
        return reference ? reference->getType() : Utils::dtypeToStr(type);
    }

    virtual bool analyze(ScopeContext* context) {
        return analyze(context, false);
    }

    virtual bool analyze(ScopeContext* context, bool valueUsed) {
        used = valueUsed;
        return true;
    }
};

/**
 * The node class holding a data type in the parse tree.
 */
struct TypeNode : public StatementNode {
    DataType type;

    TypeNode(const Location& loc, DataType type) : StatementNode(loc) {
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

    ErrorNode(const Location& loc, const string& what) {
        this->loc = loc;
        this->what = what;
        this->loc.pos -= this->loc.len - 1;
    }

    virtual string toString(int ind) {
        return string(ind, ' ') + ">> ERROR" ;
    }
    
    virtual bool analyze(ScopeContext* context);
};


#endif