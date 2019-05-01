#ifndef __CONTROL_NODES_H_
#define __CONTROL_NODES_H_

#include "basic_nodes.h"
#include "statement_nodes.h"


/**
 * The node class holding an if statement in the parse tree.
 */
struct IfNode : public StatementNode {
    ExpressionNode* cond;
    StatementNode* ifBody;
    StatementNode* elseBody;

    IfNode(const Location& loc, ExpressionNode* cond, StatementNode* ifBody, StatementNode* elseBody = NULL) : StatementNode(loc) {
        this->cond = cond;
        this->ifBody = ifBody;
        this->elseBody = elseBody;
    }

    virtual ~IfNode() {
        if (cond) delete cond;
        if (ifBody) delete ifBody;
        if (elseBody) delete elseBody;
    }

    virtual bool analyze(ScopeContext* context) {
        if (context->isGlobalScope()) {
            context->printError("if-statement is not allowed in global scope", loc);
            return false;
        }

        bool ret = true;

        context->addScope(SCOPE_IF);

        ret &= cond->analyze(context);
        ret &= ifBody->analyze(context);
        
        if (elseBody) {
            ret &= elseBody->analyze(context);
        }

        context->popScope();

        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "if (" + cond->toString() + ")\n";
        ret += ifBody->toString(ind + (dynamic_cast<BlockNode*>(ifBody) ? 0 : 4));

        if (elseBody) {
            ret += "\n" + string(ind, ' ') + "else\n";
            ret += elseBody->toString(ind + (dynamic_cast<BlockNode*>(elseBody) ? 0 : 4));
        }

        return ret;
    }
};

/**
 * The node class holding a case statement in the parse tree.
 */
struct CaseLabelNode : public StatementNode {
    ExpressionNode* expr;
    StatementNode* stmt;

    CaseLabelNode(const Location& loc, ExpressionNode* expr, StatementNode* stmt) : StatementNode(loc) {
        this->expr = expr;
        this->stmt = stmt;
    }

    virtual ~CaseLabelNode() {
        if (expr) delete expr;
        if (stmt) delete stmt;
    }

    virtual bool analyze(ScopeContext* context) {
        bool ret = true;

        if (expr) {
            ret = expr->analyze(context);

            if (ret && !expr->isConst) {
                context->printError("constant expression required in case label", expr->loc);
                ret = false;
            }
        }

        if (!context->hasSwitchScope()) {
            context->printError("case label not within switch statement", loc);
            ret = false;
        }

        ret &= stmt->analyze(context);

        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(max(0, ind - 4), ' ') + (expr ? "case " + expr->toString() + ":\n" : "default:\n");
        ret += stmt->toString(ind);
        return ret;
    }
};

/**
 * The node class holding a switch statement in the parse tree.
 */
struct SwitchNode : public StatementNode {
    ExpressionNode* cond;
    StatementNode* body;

    SwitchNode(const Location& loc, ExpressionNode* cond, StatementNode* body) : StatementNode(loc) {
        this->cond = cond;
        this->body = body;
    }

    virtual ~SwitchNode() {
        if (cond) delete cond;
        if (body) delete body;
    }

    virtual bool analyze(ScopeContext* context) {
        if (context->isGlobalScope()) {
            context->printError("switch-statement is not allowed in global scope", loc);
            return false;
        }

        bool ret = true;

        context->addScope(SCOPE_SWITCH);

        ret &= cond->analyze(context);

        if (!Utils::isIntegerType(cond->type)) {
            context->printError("switch quantity not an integer", cond->loc);
            ret = false;
        }

        ret &= body->analyze(context);

        context->popScope();

        return ret;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "switch (" + cond->toString() + ")\n";
        ret += body->toString(ind + (dynamic_cast<BlockNode*>(body) ? 0 : 4));
        return ret;
    }
};

#endif