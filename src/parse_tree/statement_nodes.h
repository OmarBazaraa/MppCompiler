#ifndef __STATEMENT_NODES_H_
#define __STATEMENT_NODES_H_

#include "basic_nodes.h"


/**
 * The node class holding a block of code in the parse tree.
 */
struct BlockNode : public StatementNode {
    vector<StatementNode*> statements;

    BlockNode() {

    }

    virtual ~BlockNode() {
        for (int i = 0; i < statements.size(); ++i) {
            delete statements[i];
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "{" << endl;
        for (int i = 0; i < statements.size(); ++i) {
            statements[i]->print(ind + 4);
            cout << endl;
        }
        cout << string(ind, ' ') << "}";
    }
};

/**
 * The node class holding a variable or constant declaration statement in the parse tree.
 */
struct VarDeclarationNode : public StatementNode {
    DataType type;
    string name;
    ExpressionNode* value;
    bool isConst;

    VarDeclarationNode(DataType type, const char* name, ExpressionNode* value = NULL, bool isConst = false) {
        this->type = type;
        this->name = string(name);
        this->value = value;
        this->isConst = isConst;
    }

    virtual ~VarDeclarationNode() {
        if (value) {
            delete value;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ');

        if (isConst) {
            cout << "const ";
        }

        cout << Utils::dtypeToStr(type) << ' ' << name;

        if (value) {
            cout << " = ";
            value->print(0);
        }
    }
};

/**
 * The node class holding a break statement in the parse tree.
 */
struct BreakStmtNode : public StatementNode {
    BreakStmtNode() {

    }

    virtual ~BreakStmtNode() {

    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "break;";
    }
};

/**
 * The node class holding a continue statement in the parse tree.
 */
struct ContinueStmtNode : public StatementNode {
    ContinueStmtNode() {

    }

    virtual ~ContinueStmtNode() {

    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "continue;";
    }
};

/**
 * The node class holding a return statement in the parse tree.
 */
struct ReturnStmtNode : public StatementNode {
    ExpressionNode* value;

    ReturnStmtNode(ExpressionNode* value) {
        this->value = value;
    }

    virtual ~ReturnStmtNode() {
        if (value) {
            delete value;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "return ";
        value->print(0);
        cout << ";";
    }
};

/**
 * The node class holding a case statement in the parse tree.
 */
struct CaseStmtNode : public StatementNode {
    ExpressionNode* expr;
    StatementNode* body;
    bool isDefault;

    CaseStmtNode(ExpressionNode* expr, StatementNode* body, bool isDefault = false) {
        this->expr = expr;
        this->body = body;
        this->isDefault = isDefault;
    }

    virtual ~CaseStmtNode() {
        if (expr) {
            delete expr;
        }
        if (body) {
            delete body;
        }
    }

    virtual void print(int ind = 0) {
        if (isDefault) {
            cout << string(ind, ' ') << "default:" << endl;
        } else {
            cout << string(ind, ' ') << "case ";
            expr->print(0);
            cout << ":" << endl;
        }
        if (body) {
            body->print(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
            cout << endl;
        }
    }
};

#endif