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

    virtual bool analyze(ScopeContext* context) {
        if (context->isGlobalScope()) {
            context->printError("while-statement is not allowed in global scope", loc);
            return false;
        }

        bool ret = true;

        context->addScope(SCOPE_LOOP);

        ret &= cond->analyze(context);
        ret &= body->analyze(context);

        context->popScope();

        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "while (" + cond->toString() + ") \n";
        ret += body->toString(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
        return ret;
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

    virtual bool analyze(ScopeContext* context) {
        if (context->isGlobalScope()) {
            context->printError("do-while-statement is not allowed in global scope", loc);
            return false;
        }

        bool ret = true;

        context->addScope(SCOPE_LOOP);

        ret &= cond->analyze(context);
        ret &= body->analyze(context);

        context->popScope();

        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "do\n";
        ret += body->toString(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4)) + "\n";
        ret += string(ind, ' ') + "while (" + cond->toString() + ");";
        return ret;
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

    virtual bool analyze(ScopeContext* context) {
        if (context->isGlobalScope()) {
            context->printError("for-statement is not allowed in global scope", loc);
            return false;
        }

        bool ret = true;

        context->addScope(SCOPE_LOOP);

        if (initStmt) ret &= initStmt->analyze(context);
        if (cond) ret &= cond->analyze(context);
        if (inc) ret &= inc->analyze(context);
        ret &= body->analyze(context);

        context->popScope();

        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "for (";
        ret += (initStmt ? initStmt->toString() : "") + ";";
        ret += (cond ? cond->toString() : "") + ";";
        ret += (inc ? inc->toString() : "") + ";";
        ret += ")\n";
        ret += body->toString(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
        return ret;
    }
};

#endif