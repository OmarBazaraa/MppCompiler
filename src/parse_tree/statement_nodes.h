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
 * The node class holding a variable use in the parse tree.
 */
struct VarNode : public ExpressionNode {
    string name;

    VarNode(const char* name) {
        this->name = string(name);
    }

    virtual void print(int ind = 0) {
        cout << string(ind, ' ') << name;
    }
};

#endif