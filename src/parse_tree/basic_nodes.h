#ifndef __BASIC_NODES_H_
#define __BASIC_NODES_H_

#include <iostream>
#include <string>
#include <vector>

#include "../context/quadruple_context.h"
#include "../utils/consts.h"
#include "../utils/utils.h"


//
// Prototypes
//
struct Node;
struct StatementNode;
struct ExpressionNode;
struct VarDeclarationNode;
struct CaseStmtNode;

typedef vector<StatementNode*> StmtList;
typedef vector<ExpressionNode*> ExprList;
typedef vector<VarDeclarationNode*> VarList;
typedef vector<CaseStmtNode*> CaseList;


/**
 * The base class of all parse tree nodes.
 */
struct Node {

    virtual ~Node() {

    }

    virtual void print(int ind = 0) {
        
    }
    
    virtual void generateQuad(QuadrupleContext* quadContext) {
        
    }
};

/**
 * The base class of all statement nodes in the parse tree.
 */
struct StatementNode : Node {
    Location loc;

    StatementNode() {

    }

    StatementNode(const Location& loc) {
        this->loc = loc;
    }

    virtual ~StatementNode() {

    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << ";" ;
    }
    
    virtual void generateQuad(QuadrupleContext* quadContext) {
        
    }
};

/**
 * The base class of all expression nodes in the parse tree.
 */
struct ExpressionNode : public StatementNode {

    ExpressionNode() {

    }

    ExpressionNode(const Location& loc) : StatementNode(loc) {
        this->loc = loc;
    }

    virtual ~ExpressionNode() {

    }
};

/**
 * The node class representing a syntax error statement.
 */
struct ErrorNode : public StatementNode {
    int lineNum;
    int cursorPos;
    int tokenLen;
    string what;

    ErrorNode(int lineNum, int cursorPos, int tokenLen, const string& what) {
        this->lineNum = lineNum;
        this->cursorPos = cursorPos;
        this->tokenLen = tokenLen;
        this->what = what;
    }

    ~ErrorNode() {

    }
};

#endif