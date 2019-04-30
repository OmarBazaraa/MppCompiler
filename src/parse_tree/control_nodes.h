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
 * The node class holding a switch statement in the parse tree.
 * 
 * TODO: redefine switch case grammar.
 */
struct SwitchNode : public StatementNode {
    ExpressionNode* cond;
    CaseList caseList;

    SwitchNode(const Location& loc, ExpressionNode* cond, const CaseList& caseList) : StatementNode(loc) {
        this->cond = cond;
        this->caseList = caseList;
    }

    virtual ~SwitchNode() {
        if (cond) delete cond;
        
        for (int i = 0; i < caseList.size(); ++i) {
            delete caseList[i];
        }
    }

    virtual bool analyze(ScopeContext* context) {
        return true;
    }

    virtual string toString(int ind = 0) {
        string ret = string(ind, ' ') + "switch (" + cond->toString() + ") {\n";
        for (int i = 0; i < caseList.size(); ++i) {
            ret += caseList[i]->toString(ind + 4);
        }
        ret += string(ind, ' ') + "}";
        return ret;
    }
};

#endif