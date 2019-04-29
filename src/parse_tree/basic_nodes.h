#ifndef __BASIC_NODES_H_
#define __BASIC_NODES_H_

#include <iostream>
#include <string>
#include <vector>

#include "../context/scope_context.h"

#include "../symbol_table/symbol_table.h"

#include "../utils/consts.h"
#include "../utils/utils.h"


//
// Prototypes
//
struct StatementNode;
struct ExpressionNode;
struct VarDeclarationNode;
struct CaseStmtNode;
struct FunctionNode;

typedef vector<StatementNode*> StmtList;
typedef vector<ExpressionNode*> ExprList;
typedef vector<VarDeclarationNode*> VarList;
typedef vector<CaseStmtNode*> CaseList;


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

    virtual bool analyze(ScopeContext* context) {
        return true;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << ";" ;
    }
};

/**
 * The base class of all expression nodes in the parse tree.
 */
struct ExpressionNode : public StatementNode {

    ExpressionNode() {}

    ExpressionNode(const Location& loc) : StatementNode(loc) {}
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

    virtual bool analyze(ScopeContext* context) {
        context->printError(what, loc);
        return false;
    }

    virtual void print(int ind) {
        cout << string(ind, ' ') << ">> ERROR" ;
    }
};

#endif