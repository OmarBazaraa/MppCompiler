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

    IfNode(ExpressionNode* cond, StatementNode* ifBody = NULL, StatementNode* elseBody = NULL) {
        this->cond = cond;
        this->ifBody = ifBody;
        this->elseBody = elseBody;
    }

    virtual ~IfNode() {
        if (cond) {
            delete cond;
        }
        if (ifBody) {
            delete ifBody;
        }
        if (elseBody) {
            delete elseBody;
        }
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << "if (";
        cond->print(0);
        cout << ")" << endl;
        ifBody->print(ind + (dynamic_cast<BlockNode*>(ifBody) ? 0 : 4));

        if (elseBody) {
            cout << endl;
            cout << string(ind, ' ') << "else" << endl;
            elseBody->print(ind + (dynamic_cast<BlockNode*>(elseBody) ? 0 : 4));
        }
    }
};

#endif