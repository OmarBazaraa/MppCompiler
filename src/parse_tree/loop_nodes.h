#ifndef __LOOP_NODES_H_
#define __LOOP_NODES_H_

#include "basic_nodes.h"
#include "statement_nodes.h"


/**
 * The node class holding a while loop in the parse tree.
 */
struct WhileNode : public StatementNode {
    ExpressionNode* cond;
    StatementNode* body;

    WhileNode(const Location& loc, ExpressionNode* cond, StatementNode* body) : StatementNode(loc) {
        this->cond = cond;
        this->body = body;
    }

    virtual ~WhileNode() {
        if (cond) delete cond;
        if (body) delete body;
    }

    virtual bool analyze(Context* context) {
        bool ret = true;

        context->addScope(SCOPE_LOOP);

        ret &= cond->analyze(context);
        ret &= body->analyze(context);

        context->popScope();

        return ret;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "while (";
        cond->print(0);
        cout << ") " << endl;
        body->print(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
    }
};

/**
 * The node class holding a do-while loop in the parse tree.
 */
struct DoWhileNode : public StatementNode {
    ExpressionNode* cond;
    StatementNode* body;

    DoWhileNode(const Location& loc, ExpressionNode* cond, StatementNode* body) : StatementNode(loc) {
        this->cond = cond;
        this->body = body;
    }

    virtual ~DoWhileNode() {
        if (cond) delete cond;
        if (body) delete body;
    }

    virtual bool analyze(Context* context) {
        bool ret = true;

        context->addScope(SCOPE_LOOP);

        ret &= cond->analyze(context);
        ret &= body->analyze(context);

        context->popScope();

        return ret;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "do" << endl;
        body->print(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
        cout << endl;
        cout << string(ind, ' ') << "while (";
        cond->print(0);
        cout << ");";
    }
};

/**
 * The node class holding a for loop in the parse tree.
 */
struct ForNode : public StatementNode {
    StatementNode* initStmt;
    ExpressionNode* cond;
    ExpressionNode* inc;
    StatementNode* body;

    ForNode(const Location& loc, StatementNode* initStmt, ExpressionNode* cond, ExpressionNode* inc, StatementNode* body) : StatementNode(loc) {
        this->initStmt = initStmt;
        this->cond = cond;
        this->inc = inc;
        this->body = body;
    }

    virtual ~ForNode() {
        if (initStmt) delete initStmt;
        if (cond) delete cond;
        if (inc) delete inc;
        if (body) delete body;
    }

    virtual bool analyze(Context* context) {
        bool ret = true;

        context->addScope(SCOPE_IF);

        if (initStmt) {
            ret &= initStmt->analyze(context);
        }
        if (cond) {
            ret &= cond->analyze(context);
        }
        if (inc) {
            ret &= inc->analyze(context);
        }
        
        ret &= body->analyze(context);

        context->popScope();

        return ret;
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "for (";

        if (initStmt) {
            initStmt->print(0);
        }
        cout << "; ";

        if (cond) {
            cond->print(0);
        }
        cout << "; ";

        if (inc) {
            inc->print(0);
        }

        cout << ")" << endl;

        body->print(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
    }
};

#endif